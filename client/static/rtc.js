let pc = null;
let localStream = null;

function ensurePc() {
  if (pc) return pc;
  pc = new RTCPeerConnection({ iceServers: [{ urls: 'stun:stun.l.google.com:19302' }] });
  pc.onicecandidate = (e) => {
    if (e.candidate && window.rtcBridge) {
      const obj = { candidate: e.candidate.candidate, sdpMid: e.candidate.sdpMid, sdpMLineIndex: e.candidate.sdpMLineIndex };
      window.rtcBridge.sig_local_ice(obj);
    }
  };
  pc.ontrack = (e) => {
    const v = document.getElementById('remoteVideo');
    if (v && e.streams && e.streams[0]) v.srcObject = e.streams[0];
  };
  return pc;
}

async function getLocalStream(kind) {
  if (localStream) return localStream;
  localStream = await navigator.mediaDevices.getUserMedia({ audio: true, video: kind === 'video' });
  const v = document.getElementById('localVideo');
  if (v) v.srcObject = localStream;
  return localStream;
}

window.initRtc = function(bridge) {
  bridge.sig_local_offer.connect(function(obj){});
  bridge.sig_local_answer.connect(function(obj){});
  bridge.sig_local_ice.connect(function(obj){});
};

window.createOffer = async function(kind) {
  const pc = ensurePc();
  const stream = await getLocalStream(kind);
  stream.getTracks().forEach(t => pc.addTrack(t, stream));
  const offer = await pc.createOffer();
  await pc.setLocalDescription(offer);
  return offer;
}

window.applyAnswer = async function(answer) {
  const pc = ensurePc();
  await pc.setRemoteDescription(new RTCSessionDescription(answer));
}

window.onRemoteOffer = async function(offer) {
  const pc = ensurePc();
  const stream = await getLocalStream(offer.type === 'video' ? 'video' : 'audio');
  stream.getTracks().forEach(t => pc.addTrack(t, stream));
  await pc.setRemoteDescription(new RTCSessionDescription(offer));
  const answer = await pc.createAnswer();
  await pc.setLocalDescription(answer);
  if (window.rtcBridge) {
    window.rtcBridge.sig_local_answer({ type: 'answer', sdp: answer.sdp });
  }
}

window.onRemoteAnswer = async function(answer) {
  const pc = ensurePc();
  await pc.setRemoteDescription(new RTCSessionDescription(answer));
}

window.onRemoteIce = async function(ice) {
  const pc = ensurePc();
  try {
    await pc.addIceCandidate(new RTCIceCandidate({ candidate: ice.candidate, sdpMid: ice.sdpMid, sdpMLineIndex: ice.sdpMLineIndex }));
  } catch (e) {
    console.error('addIceCandidate failed', e);
  }
} 