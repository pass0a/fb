#include "plugin.h"
#include "fbdev.h"
#include <iostream>
#include <map>
#include <memory>

struct pa_plugin gp;
class jsFrameBuffer
    :public std::enable_shared_from_this<jsFrameBuffer>{
public:
	static std::map<jsFrameBuffer*, std::shared_ptr<jsFrameBuffer>>& mgr() {
		static std::map<jsFrameBuffer*, std::shared_ptr<jsFrameBuffer>> mgr;
		return mgr;
	};

	jsFrameBuffer(pa_context* ctx,const char* path)
		:ctx_(ctx){
		fb.init(path);
	}
	virtual ~jsFrameBuffer(){
		std::cout<<"free jsFrameBuffer!!!"<<std::endl;
	}
	jsFrameBuffer* attach(){
		mgr().insert(std::make_pair(this,shared_from_this()));
		return this;
	}
	int info(){
		fbInfo& x=fb.info();
		gp.push_object(ctx_);
		gp.push_uint(ctx_,x.size);
		gp.put_prop_string(ctx_,-2,"size");
		gp.push_uint(ctx_,x.width);
		gp.put_prop_string(ctx_,-2,"width");
		gp.push_uint(ctx_,x.height);
		gp.put_prop_string(ctx_,-2,"height");
		gp.push_uint(ctx_,x.bpp);
		gp.put_prop_string(ctx_,-2,"bpp");
		gp.push_uint(ctx_,x.rlen);
		gp.put_prop_string(ctx_,-2,"rlen");
		gp.push_uint(ctx_,x.glen);
		gp.put_prop_string(ctx_,-2,"glen");
		gp.push_uint(ctx_,x.blen);
		gp.put_prop_string(ctx_,-2,"blen");
		gp.push_uint(ctx_,x.ros);
		gp.put_prop_string(ctx_,-2,"ros");
		gp.push_uint(ctx_,x.gos);
		gp.put_prop_string(ctx_,-2,"gos");
		gp.push_uint(ctx_,x.bos);
		gp.put_prop_string(ctx_,-2,"bos");
		return 1;
	}
	int read(void* buf,uint32_t len){
		fb.read(buf,len);
		return 0;
	}
private:
	framebuffer fb;
	pa_context* ctx_;
};
int fbOpen(pa_context* ctx) {
	if(gp.is_string(ctx, 0)){
		std::shared_ptr<jsFrameBuffer> pr(new jsFrameBuffer(ctx,gp.get_string(ctx,0)));
		gp.push_pointer(ctx, pr->attach());
		return 1;
	}
	return 0;
}
int fbInfo(pa_context* ctx) {
	if(!gp.is_pointer(ctx,0)){
		return 0;
	}
	std::map<jsFrameBuffer*,std::shared_ptr<jsFrameBuffer>>::iterator it=jsFrameBuffer::mgr().find(static_cast<jsFrameBuffer*>(gp.get_pointer(ctx,0)));
	if(it!=jsFrameBuffer::mgr().end()){
		return it->second->info();
	}
	return 0;
}
int fbRead(pa_context* ctx) {
	if(!gp.is_pointer(ctx,0) || !gp.is_buffer_data(ctx,1)){
		return 0;
	}
	std::map<jsFrameBuffer*,std::shared_ptr<jsFrameBuffer>>::iterator it=jsFrameBuffer::mgr().find(static_cast<jsFrameBuffer*>(gp.get_pointer(ctx,0)));
	if(it!=jsFrameBuffer::mgr().end()){
		int len = 0;
		void* buf=gp.get_buffer_data(ctx, 1, &len);
		return it->second->read(buf,len);
	}	
	return 0;
}
static const pa_function_list_entry my_module_funcs[] = {
	{ "open", fbOpen, PA_VARARGS /*nargs*/ },
	{ "info", fbInfo, PA_VARARGS /*nargs*/ },
	{ "read", fbRead, PA_VARARGS /*nargs*/ },
	{ NULL, NULL, 0 }
};
extern "C" int passoa_init(pa_plugin p) {
    gp = p;
    gp.put_function_list(p.ctx, -1, my_module_funcs);
    return 0;
}
extern "C" int passoa_exit() {
	return 0;
}
