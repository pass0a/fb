const fb=require("fb");

var ffb=new fb.FrameBuffer("/dev/fb0");
var info=ffb.info();
console.log(info);