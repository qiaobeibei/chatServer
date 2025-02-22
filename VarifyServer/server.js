const grpc = require('@grpc/grpc-js')
const message_proto = require('./proto')
const const_module = require('./const')
const {v4:uuidv4} = require('uuid')
const emailModule = require('./email')
const redis_module = require('./redis')

async function GetVarifyCode(call, callback) {
    console.log("email is ", call.request.email)
    try{
        // 根据 key 获取 value，这里根据你的邮箱判断先前是否已生成验证码，key = code_"你的邮箱"
        let query_res = await redis_module.GetRedis(const_module.code_prefix + call.request.email);
        console.log("query_res is ",query_res)
        let uniqueId = query_res
        if(query_res == null){ // 若是第一次生成验证码，则生成
            uniqueId = uuidv4();
            if (uniqueId.length > 4) {
                uniqueId = uniqueId.substring(0, 4); // 只截取验证码的前四位
            } 
            // 设置邮箱以及对应的验证码，并设置过期时间
            let bres = await redis_module.SetRedisExpire(const_module.code_prefix+call.request.email, uniqueId,360)
            if(!bres){
                callback(null, { email:  call.request.email,
                    error:const_module.Errors.RedisErr
                });
                return;
            }
        }

        console.log("uniqueId is ", uniqueId)
        let text_str =  '您的验证码为'+ uniqueId +'请三分钟内完成注册'
        //发送邮件
        let mailOptions = {
            from: '15991599205@163.com',
            to: call.request.email,
            subject: '验证码',
            text: text_str,
        };

        let send_res = await emailModule.SendMail(mailOptions);
        console.log("send res is ", send_res)

        if(!send_res){
            callback(null, { email:  call.request.email,
                error:const_module.Errors.Exception
            });
            return;
        }

        callback(null, { email:  call.request.email,
            error:const_module.Errors.Success
        }); 
        
    }catch(error){
        console.log("catch error is ", error)
        callback(null, { email:  call.request.email,
            error:const_module.Errors.Exception
        }); 
    }
}

function main() {
    var server = new grpc.Server()
    server.addService(message_proto.VarifyService.service, { GetVarifyCode: GetVarifyCode })
    server.bindAsync('127.0.0.1:50051', grpc.ServerCredentials.createInsecure(), () => {
        server
        console.log('grpc server started')        
    })
}

main()