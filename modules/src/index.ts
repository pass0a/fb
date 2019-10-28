const fb = require('./fb.passoa');
export class FrameBuffer {
	inst: any = null;
	constructor(path: string) {
		this.inst = fb.open(path);
	}

	read(buf: Buffer) {
		return fb.read(this.inst, buf);
	}
	info() {
		return fb.info(this.inst);
	}
}
