#pragma once
#include "const.h"
#include <thread>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>


// 该类用于保存 mysql 上一次操作的时间，用于判断连接时间以便延迟或断开等操作
// 比如每过 1 小时，通过该类记录的时间，向 mysql 发送一个请求，以防 mysql 因连接时间过长将其连接断开
class SqlConnection {
public:
    SqlConnection(sql::Connection* con,int64_t lasttime):_con(con),_last_oper_time(lasttime){}
    std::unique_ptr<sql::Connection> _con;
    int64_t _last_oper_time;
};

class MySqlPool {
public:
    MySqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize)
        : url_(url), user_(user), pass_(pass), schema_(schema), poolSize_(poolSize), b_stop_(false) {
        try {
            for (int i = 0; i < poolSize_; ++i) {
                sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
                auto* con = driver->connect(url_, user_, pass_);
                con->setSchema(schema_);
                // 获取当前时间戳
                auto currentTime = std::chrono::system_clock::now().time_since_epoch();
                // 将时间转换为秒
                long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
                pool_.push(std::make_unique<SqlConnection>(con, timestamp));
            }

            // 单独建立一个检测线程，该线程中，每隔 60 s检测一次，向mysql发送请求，避免其关闭连接
            check_thread_ = std::thread([this]() {
                while (!b_stop_) {
                    checkConnection();
                    std::this_thread::sleep_for(std::chrono::seconds(60));
                }
                });
        }
        catch (sql::SQLException& e) {
            // 处理异常
            std::cout << "mysql pool init failed， err is " << e.what() << std::endl;
        }
    }
    std::unique_ptr<SqlConnection> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] {
            if (b_stop_) {
                return true;
            }
            return !pool_.empty(); });
        if (b_stop_) {
            return nullptr;
        }
        std::unique_ptr<SqlConnection> con(std::move(pool_.front()));
        pool_.pop();
        return con;
    }

    void returnConnection(std::unique_ptr<SqlConnection> con) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (b_stop_) {
            return;
        }
        pool_.push(std::move(con));
        cond_.notify_one();
    }

    void Close() {
        b_stop_ = true;
        cond_.notify_all();
    }

    ~MySqlPool() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!pool_.empty()) {
            pool_.pop();
        }
    }

    void checkConnection() {
        std::lock_guard<std::mutex> guard(mutex_);
        int poolsize = pool_.size();
        // 获取当前时间戳
        auto currentTime = std::chrono::system_clock::now().time_since_epoch();
        // 将时间戳转换为秒
        long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
        for (int i = 0; i < poolsize; i++) {
            auto con = std::move(pool_.front());
            pool_.pop();

            // 通过 RAII 技术，当 pool 中 pop 出的连接用完后，自动将其放回 pool 中
            Defer defer([this, &con]() {
                pool_.push(std::move(con));
                });

            if (timestamp - con->_last_oper_time < 60) {
                continue;
            }

            // 连接时间大于 60s 时，执行下列过程，向 mysql 申请新的连接，以免被 mysql 主动断开
            try {
                std::unique_ptr<sql::Statement> stmt(con->_con->createStatement());
                stmt->executeQuery("SELECT 1");
                con->_last_oper_time = timestamp;
                //std::cout << "execute timer alive query , cur is " << timestamp << std::endl;
            }
            catch (sql::SQLException& e) {
                std::cout << "Error keeping connection alive: " << e.what() << std::endl;
                // 重新创建连接并替换旧的连接
                sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
                auto* newcon = driver->connect(url_, user_, pass_);
                newcon->setSchema(schema_);
                con->_con.reset(newcon);
                con->_last_oper_time = timestamp;
            }
        }
    }
private:
    std::string url_;
    std::string user_;
    std::string pass_;
    std::string schema_;
    int poolSize_;
    std::queue<std::unique_ptr<SqlConnection>> pool_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> b_stop_;
    std::thread check_thread_; // 每过 1 小时向 mysql 发送请求，以防其断开连接
};

// 存储用户信息
struct UserInfo {
    std::string name;
    std::string pwd;
    int uid;
    std::string email;
};

class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    int RegUserTransaction(const std::string& name, const std::string& email, const std::string& pwd, const std::string& icon);
    bool CheckEmail(const std::string& name, const std::string& email);
    bool UpdatePwd(const std::string& name, const std::string& newpwd);
    bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
    bool TestProcedure(const std::string& email, int& uid, std::string& name);
private:
    std::unique_ptr<MySqlPool> pool_;
};