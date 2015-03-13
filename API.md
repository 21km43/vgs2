# API�֐����t�@�����X
`SUZUKI PLAN - Video Game System mk-II SR` ���񋟂���API�֐��̎d�l���L���܂��B

## (1)�f�o�C�X����

### vgs2_touch
> ��ʂ̃^�b�`�i�}�E�X�̃N���b�N�j��Ԃ��擾���܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_touch(int* s,int* cx,int* cy,int* dx,int* dy);
>
> `����`
> - s: ��^�b�`����0, �^�b�`���͔�0
> - cx: �^�b�`���Ă���X���W�i��^�b�`���͕s��l�j
> - cy: �^�b�`���Ă���Y���W�i��^�b�`���͕s��l�j
> - dx: �X���C�h����X���W�����i��^�b�`���͕s��l�j
> - dy: �X���C�h����Y���W�����i��^�b�`���͕s��l�j

### vgs2_setPause
> �|�[�Y��Ԃ̐ݒ�A�������s���܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_setPause(unsigned char c);
>
> `����`
> - c: �|�[�Y��Ԃɂ���ꍇ��1�A��������ꍇ��0���w�肷��

## (2)�t�@�C�����o��

### vgs2_fopen
> - �t�@�C�����J���܂��B
> - �t�@�C�����J�����́A�W���֐�fopen�̑���ɂ��̊֐���p���Ă��������B
> - �t�@�C���̓ǂݏ�������鑀��ɂ́A�W���֐��ifread, fwrite, fclose���j��p���Ă��������B
> - �t�@�C�����ɂ́A�f�B���N�g�����܂߂Ȃ��ł��������B
>
> `�v���g�^�C�v`
>
> FILE* vgs2_fopen(const char* n,const char* p);
>
> `�����E�߂�l`
> - fopen�֐��Ɠ����ł��B

## (3)���w
GameDaddy�̐��w�́A�ʏ�̐��w�֐��ƈႢ�A������S�ČŒ菬���_�������ň����܂��B

### vgs2_abs
> - ��Βl�����߂�}�N���ł��B
>
> `�}�N���錾`
>
> ��define vgs2_abs(x) (x>=0?(x):-(x))

### vgs2_sgn
> - �w��l������or�[���̏ꍇ��1�A�����̏ꍇ��-1��Ԃ��}�N���ł��B
> - ��ʓI��sgn�֐��ƈႢ�[����Ԃ��܂���B
>
> `�}�N���錾`
>
> ��define vgs2_sgn(x) (x>=0?(1):(-1))

### vgs2_rad
> - 2�_�Ԃ̊p�x�����W�A���l�~100�i0�`627�������_�ȉ��؂�̂āj�ŋ��߂܂��B
>
> `�v���g�^�C�v`
>
> int vgs2_rad(int x1,int y1,int x2,int y2);
>
> `����`
> - x1, y1: �_1�̍��W
> - x2, y2: �_2�̍��W
>
> `�߂�l`
> - 0�`627�i���W�A���l�~100�j

### vgs2_deg
> - 2�_�Ԃ̊p�x�i0�`359�j�����߂܂��B
>
> `�v���g�^�C�v`
>
> int vgs2_deg(int x1,int y1,int x2,int y2);
>
> `����`
> - x1, y1: �_1�̍��W
> - x2, y2: �_2�̍��W
>
> `�߂�l`
> - 0�`359

### vgs2_deg2rad
> - �p�x�����W�A���l�~100�i0�`627�������_�ȉ��؂�̂āj�ɕϊ����܂��B
>
> `�v���g�^�C�v`
>
> int vgs2_deg2rad(int deg);
>
> `����`
> - deg: �p�x
>
> `�߂�l`
> - 0�`627�i���W�A���l�~100�j

### vgs2_rad2deg
> - ���W�A���l�~100���p�x�i0�`359�j�ɕϊ����܂��B
>
> `�v���g�^�C�v`
>
> int vgs2_rad2deg(int rad);
>
> `����`
> - rad: ���W�A���l�~100
>
> `�߂�l`
> - 0�`359

### vgs2_rands
> - �^�����������������܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_rands();

### vgs2_rand
> - �^�������l�����߂܂��B
>
> `�v���g�^�C�v`
>
> int vgs2_rand();
>
> `�߂�l`
> - 0�`2147483647

### vgs2_sin
> - ���W�A���l�~100����T�C���l�~256�����߂܂��B
>
> `�v���g�^�C�v`
>
> int vgs2_sin(int r);
>
> `����`
> - r: ���W�A���l�~100
>
> `�߂�l`
> - sin(r��100)�~256

### vgs2_cos
> - ���W�A���l�~100����R�T�C���l�~256�����߂܂��B
>
> `�v���g�^�C�v`
>
> int vgs2_cos(int r);
>
> `����`
> - r: ���W�A���l�~100
>
> `�߂�l`
> - cos(r��100)�~256

## (4)���ʉ�

### vgs2_eff
> - ���ʉ����Đ����܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_eff(unsigned char n);
>
> `����`
> - n: ESLOT�̃X���b�g�ԍ�

### vgs2_effstop
> - ����̌��ʉ��̍Đ����~���܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_effstop(unsigned char n);
>
> `����`
> - n: ESLOT�̃X���b�g�ԍ�

### vgs2_effstopA
> - �S�Ă̌��ʉ��̍Đ����~���܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_effstopA();

### vgs2_setmute
> - �~���[�g��Ԃ�ݒ�or�������܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_setmute(unsigned char n);
>
> `����`
> - n: 0=�����A1=�ݒ�

### vgs2_getmute
> - �~���[�g��Ԃ��擾���܂��B
>
> `�v���g�^�C�v`
>
> unsigned char vgs2_getmute();
>
> `�߂�l`
> - �~���[�g��ԁi0=�����A1=�ݒ�j

## (5)�O���t�B�b�N�X

### vgs2_pallet
> - �x�[�X�J���[��ݒ肵�܂��B
> - �x�[�X�J���[�́A���m�N���\���̊�b�ƂȂ�F�̂��Ƃł��B
> - ��ʂ́A�{�֐��Őݒ肵���x�[�X�J���[����ɂ���8�i���̃��m�N���ŕ\������܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_pallet(int c);
>
> `����`
> - c: �x�[�X�J���[�i1�`7�j
>   - 1:blue (��)
>   - 2:red (��)
>   - 3:magenta (��)
>   - 4:green (��)
>   - 5:cyan (���F)
>   - 6:yellow (���F)
>   - 7:white (��)

### vgs2_pallet256
> - 8bit�J���[�i256�F�j���[�h�ɂ��܂��B
> - �p���b�g���̓f�[�^�X���b�g�Ɏ��K�v������܂��B
> - �f�[�^�X���b�g�ɂ́A1�F������32bit�iARGB�j�̐F��� �~ 256�F�i���v `1024byte` �j�̃o�C�i���f�[�^���i�[����Ă���K�v������܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_pallet256(unsigned char n);
>
> `����`
> - n: �p���b�g��񂪊i�[����Ă���f�[�^�X���b�g�ԍ�

### vgs2_interlace
> - �C���^���[�X�E�X�L������ON/OFF��ݒ肵�܂��B
> - �A�v���N�����̃f�t�H���g��ON�ł��B
>
> `�v���g�^�C�v`
>
> void vgs2_interlace(int i);
>
> `����`
> - i: �C���^���[�X��ON(1)�܂���OFF(0)

### vgs2_clear
> - BG�ʂ��N���A���܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_clear(unsigned char c);
>
> `����`
> 
> - c: �p���b�g�ԍ��i0�`7�j

### vgs2_scroll
> - BG�ʂ��X�N���[���i���X�^�X�N���[���j���܂��B
>
> `�v���g�^�C�v`
>
> void vgs2_scroll(int x,int y);
>
> `����`
> 
> - x,y: �X�N���[���l�i�s�N�Z���P�ʁj

### vgs2_pixel
> - �_��`�悵�܂��B
>
> `�v���g�^�C�v`
> - void vgs2_pixel(unsigned char* p,int x,int y,unsigned char c);
> - ��define vgs2_pixelBG(X,Y,C) vgs2_pixel(_vram.bg,X,Y,C)
> - ��define vgs2_pixelSP(X,Y,C) vgs2_pixel(_vram.sp,X,Y,C)
>
> `����`
> - p: _vram.bg(BG��) or _vram.sp(�X�v���C�g��)
> - x,y: ���W
> - c: �p���b�g�ԍ�

### vgs2_line
> - ����`�悵�܂��B
>
> `�v���g�^�C�v`
> - void vgs2_line(unsigned char* p,int fx,int fy,int tx,int ty,unsigned char c);
> - ��define vgs2_lineBG(FX,FY,TX,TY,C) vgs2_line(_vram.bg,FX,FY,TX,TY,C)
> - ��define vgs2_lineSP(FX,FY,TX,TY,C) vgs2_line(_vram.sp,FX,FY,TX,TY,C)
>
> `����`
> - p: _vram.bg(BG��) or _vram.sp(�X�v���C�g��)
> - fx,fy: ���W(from)
> - tx,ty: ���W(to)
> - c: �p���b�g�ԍ�

### vgs2_circle
> - �~��`�悵�܂��B
>
> `�v���g�^�C�v`
> - void vgs2_circle(char*p, int x,int y,int r, unsigned char c);
> - ��define vgs2_circleBG(X,Y,R,C) vgs2_circle(_vram.bg,X,Y,R,C)
> - ��define vgs2_circleSP(X,Y,R,C) vgs2_circle(_vram.sp,X,Y,R,C)
>
> `����`
> - p: _vram.bg(BG��) or _vram.sp(�X�v���C�g��)
> - x,y: ���W
> - r: ���a
> - c: �p���b�g�ԍ�

### vgs2_box
> - vgs2_line��p���ċ�`��`�悷��}�N���ł��B
>
> `�}�N���錾`
> ��define vgs2_boxBG(FX,FY,TX,TY,C) \
>
> vgs2_line(_vram.bg,FX,FY,TX,FY,C);\
>
> vgs2_line(_vram.bg,FX,TY,TX,TY,C);\
>
> vgs2_line(_vram.bg,FX,FY,FX,TY,C);\
>
> vgs2_line(_vram.bg,TX,FY,TX,TY,C)
>
> ��define vgs2_boxSP(FX,FY,TX,TY,C) \
>
> vgs2_line(_vram.sp,FX,FY,TX,FY,C);\
>
> vgs2_line(_vram.sp,FX,TY,TX,TY,C);\
>
> vgs2_line(_vram.sp,FX,FY,FX,TY,C);\
>
> vgs2_line(_vram.sp,TX,FY,TX,TY,C)

### vgs2_boxf
> - �h��Ԃ���`��`�悵�܂��B
>
> `�v���g�^�C�v`
> - void vgs2_boxf(unsigned char* p,int fx,int fy,int tx,int ty,unsigned char c);
> - ��define vgs2_boxfBG(FX,FY,TX,TY,C) vgs2_boxf(_vram.bg,FX,FY,TX,TY,C)
> - ��define vgs2_boxfSP(FX,FY,TX,TY,C) vgs2_boxf(_vram.sp,FX,FY,TX,TY,C)
>
> `����`
> - p: _vram.bg(BG��) or _vram.sp(�X�v���C�g��)
> - fx,fy: ���W(from)
> - tx,ty: ���W(to)
> - c: �p���b�g�ԍ�

### vgs2_putBG
> - GSLOT�̓��e��BG�ʂ֋�`�]�����܂��B
>
> `�v���g�^�C�v`
> - void vgs2_putBG(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy);
> - void vgs2_putBG2(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy);
>
> `���l`
> - vgs2_putBG: �p���b�g0���܂߂ĕ`�悷��
> - vgs2_putBG2: �p���b�g0�𓧖��F�Ƃ��ĕ`�悷��
>
> `����`
> - n: GSLOT�̃X���b�g�ԍ�
> - sx,sy: �]�������W�iGSLOT�j
> - xs,ys: �T�C�Y
> - dx,dy: �]������W�iBG�ʁj

### vgs2_putSP
> - GSLOT�̓��e���X�v���C�g�ʂ֋�`�]�����܂��B
>
> `�v���g�^�C�v`
> - void vgs2_putSP(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy);
> - void vgs2_putSPM(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy,unsigned char c);
> - void vgs2_putSPH(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy);
> - void vgs2_putSPMH(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy,unsigned char c);
> - void vgs2_putSPR(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy,int r);
> - void vgs2_putSPE(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy,int dxs,int dys);
> - void vgs2_putSPRE(unsigned char n,int sx,int sy,int xs,int ys,int dx,int dy,int r,int dxs,int dys);
>
> `���l`
> - vgs2_putSPM: �񓧖��F���}�X�N�i�P�F�`��j�Ƃ���
> - vgs2_putSPH: 1/2�T�C�Y�œ]������
> - vgs2_putSPMH: vgs2_putSPM + vgs2_putSPH�i�g�ݍ��킹�j
> - vgs2_putSPR: ��]������
> - vgs2_putSPE: �g��i�k���j����
> - vgs2_putSPRE: vgs2_putSPR +�@vgs2_putSPE�i�g�ݍ��킹�j 
>
> `����`
> - n: GSLOT�̃X���b�g�ԍ�
> - sx,sy: �]�������W�iGSLOT�j
> - xs,ys: �T�C�Y
> - dx,dy: �]������W�i�X�v���C�g�ʁj
> - c: �}�X�N���̃p���b�g�ԍ�
> - r: ��]�p�i���W�A���~100�j
> - dxs,dys: �]����T�C�Y

## (6)���[�e�B���e�B

### vgs2_getdata
> - DSLOT�̓��e���擾���܂��B
>
> `�v���g�^�C�v`
>
> const char* vgs2_getdata(unsigned char n,unsigned int* size);
>
> `����`
> - n: DSLOT�̃X���b�g�ԍ�
> - size: �T�C�Y�ioutput only�j
>
> `�߂�l`
> - NULL: �w�肵��DSLOT�����݂��Ȃ�
> - NULL�ȊO: DSLOT�̐擪�|�C���^
>
> `���l`
> - �߂�l�ŕԂ����|�C���^�́A�A���C�����g�������Ă��܂���B
