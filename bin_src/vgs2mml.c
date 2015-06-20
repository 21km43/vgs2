#include "../template/miniz.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../template/vgs2.h"

#define HZ 22050

/* �}�N�� */
struct MyMacro {
	char* name;
	char* value;
	struct MyMacro* next;
};
static struct MyMacro* g_mhead=NULL;
static struct MyMacro* g_mtail=NULL;

/* �`���l���E���R�[�h */
struct Channel {
	struct _NOTE note;
	struct Channel* next;
};

/* �`���l���E�e�[�u�� */
struct ChannelTbl {
	struct Channel* head;
	struct Channel* tail;
	int oct;							/* ���� */
	int len;							/* ���� */
	int huten;							/* �����̕t�_�L�� */
	int per;							/* �L�[�I���� */
	int tone;							/* ���F */
	int vol;							/* �Ō�ɐݒ肵���{�����[�� */
};

struct Channel* vgsmml(const char* mml);

static int kbase[6];

/* MML�e�[�u�� */
struct MML {
	struct ChannelTbl ch[6+1];
	int tempo;							/* �e���|(�S�������̒���) */
} TBL;


/* �����֐� */
static void trimstring(char* src);
static int chkmname(char* name);
static int madd(int line,char* name,char* value);
static int canl(int line,int cn,char* fmt);
static int getval(char** str);
static int cadd(int cn,struct _NOTE* note);
static int merge(struct Channel* c2);

/* ���C�� */
int main(int argc,char* argv[])
{
	FILE* fp=NULL;
	char* buf=NULL;
	int* pos=NULL;
	struct MyMacro* mp;
	struct MyMacro* mpNext;
	struct _NOTE note;
	char* cp;
	int size;
	int nLine;
	int cLine;
	int i;
	int rc=0;

	/* ������� */
	if(argc<3) {
		puts("usage: vgs2mml mml-file bgm-file");
		rc=1;
		goto ENDPROC;
	}

	/* MML���I�[�v�� */
	if(NULL==(fp=fopen(argv[1],"rb"))) {
		puts("ERROR: Input file not found.");
		rc=2;
		goto ENDPROC;
	}

	/* MML�̃T�C�Y���擾 */
	fseek(fp,0,SEEK_END);
	size=(int)ftell(fp);
	fseek(fp,0,SEEK_SET);
	if(size<=0) {
		puts("ERROR: Invalid file size.");
		rc=255;
		goto ENDPROC;
	}

	/* MML�̃o�b�t�@���m�� */
	if(NULL==(buf=(char*)malloc(size+1))) {
		puts("ERROR: No memory.");
		rc=255;
		goto ENDPROC;
	}

	/* �o�b�t�@�ɓǂݍ��� */
	if(size!=fread(buf,1,size,fp)) {
		puts("ERROR: File input error.");
		rc=255;
		goto ENDPROC;
	}
	fclose(fp);
	fp=NULL;
	buf[size]='\0';

	/* �s��+�J�n�ʒu���J�E���g */
	for(nLine=1,i=0;buf[i];i++) {
		if('\n'==buf[i]) nLine++;
	}

	/* �ʒu���L�^�o�b�t�@���m�� */
	if(NULL==(pos=(int*)malloc((nLine+1)*sizeof(int)))) {
		puts("ERROR: No memory.");
		rc=255;
		goto ENDPROC;
	}

	/* �ʒu�����L�� */
	pos[0]=0;
	for(cLine=0,i=0;buf[i] && cLine<nLine;i++) {
		switch(buf[i]) {
			case '\r':	/* ���A�R�[�h */
			case '#':	/* �R�����g */
				buf[i]='\0';
				break;
			case '\t':	/* TAB */
				buf[i]=' ';
				break;
			case '\n':
				buf[i]='\0';
				cLine++;
				pos[cLine]=i+1;
				break;
		}
	}
	pos[nLine]=-1;

	/* ��͏����l��ݒ� */
	{
		struct _NOTE es={NTYPE_ENV1,0,0,0,1000};
		struct _NOTE ee={NTYPE_ENV2,0,0,0,10000};
		struct _NOTE vo={NTYPE_VOL,0,0,0,10};
		struct _NOTE mv={NTYPE_MVOL,0,0,0,10};
		if(cadd(0,&mv)) {
			rc=255;
			goto ENDPROC;
		}
		TBL.tempo=(HZ*60)/120*4;
		for(i=0;i<6;i++) {
			TBL.ch[i].oct=4;
			TBL.ch[i].len=4;
			TBL.ch[i].tone=0;
			TBL.ch[i].per=80;
			if(cadd(i,&es) || cadd(i,&ee) || cadd(i,&vo)) {
				rc=255;
				goto ENDPROC;
			}
		}
	}

	/* 1�s�Â�� */
	for(i=0;pos[i]!=-1;i++) {
		trimstring(&buf[pos[i]]);
		switch(buf[pos[i]]) {
			case '\0':	/* ��s */
				break;
			case '$':	/* �}�N����` */
				if(NULL==(cp=strchr(&buf[pos[i]],' '))) {
					printf("ERROR: Syntax error #%d: %s\n",i+1,&buf[pos[i]]);
					rc=4;
					goto ENDPROC;
				}
				*cp='\0';
				cp++;
				trimstring(cp);
				if(chkmname(&buf[pos[i]+1])) {
					printf("ERROR: Invalid variable name #%d: name=\"%s\"\n",i+1,&buf[pos[i]+1]);
					rc=4;
					goto ENDPROC;
				}
				rc=madd(i+1,&buf[pos[i]+1],cp);
				if(rc) {
					goto ENDPROC;
				}
				break;
			case 'c':	/* �`���l������` */
			case 'C':
				if(('h'!=buf[pos[i]+1] && 'H'!=buf[pos[i]+1])||!isdigit(buf[pos[i]+2])||' '!=buf[pos[i]+3]) {
					printf("ERROR: Syntax error #%d: %s\n",i+1,&buf[pos[i]]);
					rc=4;
					goto ENDPROC;
				}
				rc=canl(i+1,buf[pos[i]+2]-'0',&buf[pos[i]+4]);
				if(rc) {
					goto ENDPROC;
				}
				break;
			default:
				printf("ERROR: Syntax error #%d: %s\n",i+1,&buf[pos[i]]);
				rc=4;
				goto ENDPROC;
		}
	}

	/* �Ō��wait0��S�`���l���ɑ}�����Ă��� */
	note.type=NTYPE_WAIT;
	note.op1=0;
	note.op2=0;
	note.op3=0;
	note.val=0;
	for(i=0;i<6;i++) {
		if(cadd(i,&note)) {
			rc=255;
			goto ENDPROC;
		}
	}

	/* �`���l���������� */
	for(i=1;i<6;i++) {
		if(merge(TBL.ch[i].head)) {
			rc=255;
			goto ENDPROC;
		}
	}

	/* ���x�������݂���ꍇ�Await0���W�����v���߂ɒu�������� */
	{
		struct Channel* ch;
		int idx;
		for(idx=0,ch=TBL.ch[0].head;ch;ch=ch->next,idx++) {
			if(NTYPE_LABEL==ch->note.type) {
				break;
			}
		}
		if(ch) {
			for(;ch;ch=ch->next) {
				if(ch->note.type==NTYPE_WAIT && ch->note.val==0) {
					break;
				}
			}
			if(ch) {
				ch->note.type=NTYPE_JUMP;
				ch->note.val=idx;
			}
		}
	}

	{
		char tmp[1024];
		uLong src_len;
		uLong cmp_len;
		unsigned char* pCmp;
		char* src;
		int size;
		struct Channel* ch;
		sprintf(tmp,"%s.tmp",argv[2]);

		/* CH0���t�@�C���ɏ����o�� */
		if(NULL==(fp=fopen(tmp,"wb"))) {
			puts("ERROR: File output error.");
			rc=255;
			goto ENDPROC;
		}
		for(ch=TBL.ch[0].head;ch;ch=ch->next) {
			fwrite(&(ch->note),sizeof(struct _NOTE),1,fp);
/*			printf(",{%d,%d,%d,%d,%d}\n"
				,ch->note.type
				,(int)ch->note.op1
				,(int)ch->note.op2
				,(int)ch->note.op3
				,ch->note.val
				);*/
		}
		fclose(fp);
		fp=NULL;

		/* ��U�������ɓǂ� */
		if(NULL==(fp=fopen(tmp,"rb"))) {
			puts("ERROR: File input error.");
			rc=255;
			goto ENDPROC;
		}
		fseek(fp,0,SEEK_END);
		size=(int)ftell(fp);
		src=(char*)malloc(size);
		if(NULL==src) {
			puts("ERROR: No memory.");
			rc=255;
			goto ENDPROC;
		}
		fseek(fp,0,SEEK_SET);
		fread(src,size,1,fp);
		fclose(fp);
		fp=NULL;
#ifdef _WIN32
		DeleteFile(tmp);
#else
		unlink(tmp);
#endif

		/* ���k���� */
		{
			int cmp_status;
			src_len = (uLong)size;
			cmp_len = compressBound(src_len);
			pCmp=(unsigned char*)malloc(cmp_len);
			if(NULL==pCmp) {
				puts("ERROR: No memory.");
				rc=255;
				goto ENDPROC;
			}
			cmp_status = compress(pCmp, &cmp_len, (const unsigned char *)src, src_len);
			if (cmp_status != Z_OK) {
				puts("ERROR: Could not compressed.");
				rc=255;
				goto ENDPROC;
			}
		}

		/* ���k�����t�@�C�������� */
		if(NULL==(fp=fopen(argv[2],"wb"))) {
			puts("ERROR: File output error.");
			rc=255;
			goto ENDPROC;
		}
		fwrite(pCmp,cmp_len,1,fp);
		fclose(fp);
		fp=NULL;
	}

ENDPROC:
	for(mp=g_mhead;mp;mp=mpNext) {
		mpNext=mp->next;
		free(mp->name);
		free(mp->value);
		free(mp);
	}
	if(pos) free(pos);
	if(buf) free(buf);
	if(fp) fclose(fp);
	return rc;
}

/* �g���� */
static void trimstring(char* src)
{
	int i,j;
	int len;
	/* �O���̋󔒂��g���� */
	for(i=0;' '==src[i];i++);
	if(i) {
		for(j=0;src[i]!='\0';j++,i++) {
			src[j]=src[i];
		}
		src[j]='\0';
	}
	/* �L��̋󔒂��g���� */
	for(len=(int)strlen(src)-1;0<=len && ' '==src[len];len--) {
		src[len]='\0';
	}
}

/* �ϐ����̃`�F�b�N */
static int chkmname(char* name)
{
	int i;
	/* �A���t�@�x�b�g/�����ȊO�͋֎~ */
	for(i=0;name[i];i++) {
		if(!isalpha(name[i]) && !isdigit(name[i])) {
			return -1;
		}
	}
	/* �󕶎���֎~ */
	if(0==i) {
		return -1;
	}
	return 0;
}

/* �}�N���ǉ� */
static int madd(int line,char* name,char* value)
{
	struct MyMacro* mp;
	/* �d�����̃`�F�b�N */
	for(mp=g_mhead;mp;mp=mp->next) {
		if(0==strcmp(name,mp->name)) {
			printf("ERROR: Duplicate variable name #%d: name=\"%s\"\n",line,name);
			return 4;
		}
	}
	/* �̈�m�� */
	mp=(struct MyMacro*)malloc(sizeof(struct MyMacro));
	if(NULL==mp) {
		puts("ERROR: No memory.");
		return 255;
	}

	mp->name=(char*)malloc(strlen(name)+1);
	if(NULL==mp->name) {
		puts("ERROR: No memory.");
		free(mp);
		return 255;
	}
	strcpy(mp->name,name);

	mp->value=(char*)malloc(strlen(value)+1);
	if(NULL==mp->value) {
		puts("ERROR: No memory.");
		free(mp->name);
		free(mp);
		return 255;
	}
	strcpy(mp->value,value);

	mp->next=NULL;

	/* �A�� */
	if(NULL==g_mtail) {
		g_mhead=mp;
		g_mtail=mp;
	} else {
		g_mtail->next=mp;
		g_mtail=mp;
	}
	return 0;
}

/* �`���l�����̉�� */
static int canl(int line,int cn,char* fmt)
{
	int rc=0;
	int i,j,k,l,m,n;
	char* cp;
	char* cp2;
	char* cbuf;
	char* cbuf2;
	struct MyMacro* mp;
	struct _NOTE note;

	trimstring(fmt);

	/* ��͗̈���m�� */
	if(NULL==(cbuf=(char*)malloc(strlen(fmt)+1))) {
		puts("ERROR: No memory.");
		return 255;
	}
	strcpy(cbuf,fmt);

	/* �`���l���ԍ��͈̔̓`�F�b�N */
	if(cn<0 || 5<cn) {
		printf("ERROR: Invalid channel number #%d: number=%d\n",line,cn);
		rc=4;
		goto ENDPROC;
	}

	/* �}�N���W�J���� */
	while(NULL!=(cp=strchr(cbuf,'('))) {
		if(NULL==(cp2=strchr(cp+1,')'))) {
			printf("ERROR: Syntax error #%d: There is no \')\' corresponding to \'(\'.\n",line);
			rc=4;
			goto ENDPROC;
		}
		*cp='\0';
		*cp2='\0';
		cp++;
		cp2++;
		/* �ϐ����T�� */
		for(mp=g_mhead;mp;mp=mp->next) {
			if(0==strcmp(cp,mp->name)) {
				break;
			}
		}
		if(NULL==mp) {
			printf("ERROR: Undeclared variable was specified #%d: name=\"%s\"",line,cp);
			rc=4;
			goto ENDPROC;
		}
		/* �W�J��̒������v�Z */
		i=(int)(strlen(cbuf)+strlen(mp->value)+strlen(cp2));
		if(NULL==(cbuf2=(char*)malloc(i+1))) {
			puts("ERROR: No memory.");
			rc=255;
			goto ENDPROC;
		}
		/* �W�J��̕������cbuf�ɃZ�b�g */
		strcpy(cbuf2,cbuf);
		strcat(cbuf2,mp->value);
		strcat(cbuf2,cp2);
		free(cbuf);
		cbuf=cbuf2;
	}

	/* �󔒂�S�č폜 */
	trimstring(cbuf);
	while(NULL!=(cp=strchr(cbuf,' '))) {
		for(;*cp!='\0';cp++) {
			*cp=*(cp+1);
		}
	}

	/* �������͑啶���ɕϊ� */
	for(cp=cbuf;*cp;cp++) {
		if('a'<=*cp && *cp<='z') {
			(*cp)-='a'-'A';
		}
	}

	/* ����� */
	for(cp=cbuf;*cp;cp++) {
		if('A'<=*cp && *cp<='G') {	/* ���� */
			/* ������n�ɋ��߂�(�I�N�^�[�u0��A��0�ƂȂ�) */
			n=TBL.ch[cn].oct*12;
			switch(*cp) {
				case 'C': n-=9; break;
				case 'D': n-=7; break;
				case 'E': n-=5; break;
				case 'F': n-=4; break;
				case 'G': n-=2; break;
				case 'A': ;     break;
				case 'B': n+=2; break;
			}
			for(;'+'==*(cp+1)||'-'==*(cp+1);cp++) {
				if('+'==*(cp+1)) {
					n++;
				} else {
					n--;
				}
			}
			n+=kbase[cn];
			if(n<0) n=0;
			if(84<n) n=84;
			/* ����(�����P��)��i/j�ɋ��߂� */
			cp++;
			i=getval(&cp);
			if(i<0) {
				cp--;
				i=TBL.ch[cn].len;
				j=TBL.ch[cn].huten;
			}
			else {
				if('.'==*(cp+1)) {
					cp++;
					j=1;
				} else {
					j=0;
				}
			}
			/* �e���|�ɑΉ����钷����k�ɋ��߂� */
			k=TBL.tempo/i;
			if(j) {
				j=i*2;
				k+=TBL.tempo/j;
			}
			/* �^�C�����Z */
			while('^'==*(cp+1)) {
				cp+=2;
				i=getval(&cp);
				if(i<0) {
					cp--;
					i=TBL.ch[cn].len;
					j=TBL.ch[cn].huten;
				} else {
					if('.'==*(cp+1)) {
						cp++;
						j=1;
					} else {
						j=0;
					}
				}
				/* �^�C���̒������e���|�ɑΉ����钷����k�ɉ��Z */
				k+=TBL.tempo/i;
				if(j) {
					j=i*2;
					k+=TBL.tempo/j;
				}
			}
			/* keyon/keyoff�̒�����l/m�ɋ��߂� */
			l=k*TBL.ch[cn].per/100;
			m=k-l;
			if(m==0) {
				m++;
				l--;
			}
			/* keyon/keyoff�̃m�[�g��ǉ� */
			note.type=NTYPE_KEYON;
			note.op1=(unsigned char)cn;
			note.op2=(unsigned char)TBL.ch[cn].tone;
			note.op3=(unsigned char)n;
			note.val=0;
			if(0!=(rc=cadd(cn,&note))) {
				goto ENDPROC;
			}
			note.type=NTYPE_WAIT;
			note.op1=0;
			note.op2=0;
			note.op3=0;
			note.val=l;
			if(0!=(rc=cadd(cn,&note))) {
				goto ENDPROC;
			}
			note.type=NTYPE_KEYOFF;
			note.op1=cn;
			note.op2=0;
			note.op3=0;
			note.val=0;
			if(0!=(rc=cadd(cn,&note))) {
				goto ENDPROC;
			}
			note.type=NTYPE_WAIT;
			note.op1=0;
			note.op2=0;
			note.op3=0;
			note.val=m;
			if(0!=(rc=cadd(cn,&note))) {
				goto ENDPROC;
			}
		}
		else if('R'==*cp) {			/* �x�� */
			/* ����(�����P��)��i/j�ɋ��߂� */
			cp++;
			i=getval(&cp);
			if(i<0) {
				cp--;
				i=TBL.ch[cn].len;
				j=TBL.ch[cn].huten;
			}
			else {
				if('.'==*(cp+1)) {
					cp++;
					j=1;
				} else {
					j=0;
				}
			}
			/* �e���|�ɑΉ����钷����k�ɋ��߂� */
			k=TBL.tempo/i;
			if(j) {
				j=i*2;
				k+=TBL.tempo/j;
			}
			/* WAIT�m�[�g��ݒ� */
			note.type=NTYPE_WAIT;
			note.op1=0;
			note.op2=0;
			note.op3=0;
			note.val=k;
			if(0!=(rc=cadd(cn,&note))) {
				goto ENDPROC;
			}
		}
		else if('P'==*cp) {		/* �s�b�`�ω� */
			cp++;
			if(*cp=='-') {
				cp++;
				i=getval(&cp);
				if(i<0) {
					printf("ERROR: Invalid value #%d: operand=\"P-%d\"\n",line,i);
					rc=4;
					goto ENDPROC;
				}
				note.type=NTYPE_PDOWN;
				note.op1=cn;
				note.op2=0;
				note.op3=0;
				note.val=i;
				if(0!=(rc=cadd(cn,&note))) {
					goto ENDPROC;
				}
			}
			else {
				printf("ERROR: Invalid operand #%d: operand=\"P%c\"\n",line,*cp);
				rc=4;
				goto ENDPROC;
			}
		}
		else if('\\'==*cp) {		/* �G���x���[�v */
			cp++;
			if(*cp=='S') {
				cp++;
				i=getval(&cp);
				if(i<1) {
					printf("ERROR: Invalid value #%d: operand=\"\\S%d\"\n",line,i);
					rc=4;
					goto ENDPROC;
				}
				note.type=NTYPE_ENV1;
				note.op1=cn;
				note.op2=0;
				note.op3=0;
				note.val=i;
				if(0!=(rc=cadd(cn,&note))) {
					goto ENDPROC;
				}
			}
			else if(*cp=='E') {
				cp++;
				i=getval(&cp);
				if(i<1) {
					printf("ERROR: Invalid value #%d: operand=\"\\E%d\"\n",line,i);
					rc=4;
					goto ENDPROC;
				}
				note.type=NTYPE_ENV2;
				note.op1=cn;
				note.op2=0;
				note.op3=0;
				note.val=i;
				if(0!=(rc=cadd(cn,&note))) {
					goto ENDPROC;
				}
			}
			else {
				printf("ERROR: Unknown operand #%d: operand=\"\\%c\"\n",line,*cp);
				rc=4;
				goto ENDPROC;
			}
		}
		else if('%'==*cp) {			/* �L�[�I���� */
			cp++;
			i=getval(&cp);
			if(i<1 || 100<i) {
				printf("ERROR: Invalid operand value #%d: operand=\"%%%d\"\n",line,i);
				rc=4;
				goto ENDPROC;
			}
			TBL.ch[cn].per=i;
		}
		else if('@'==*cp) {			/* ���F */
			cp++;
			i=getval(&cp);
			if(i<0) {
				printf("ERROR: Operand value is not specified #%d: operand=\'@\'\n",line);
				rc=4;
				goto ENDPROC;
			}
			TBL.ch[cn].tone=i;
		}
		else if('T'==*cp) {			/* �e���| */
			cp++;
			i=getval(&cp);
			if(i<1) {
				printf("ERROR: Operand value is not specified #%d: operand=\'T\'\n",line);
				rc=4;
				goto ENDPROC;
			}
			TBL.tempo=(HZ*60)/i*4;
		}
		else if('L'==*cp) {			/* ���� */
			cp++;
			i=getval(&cp);
			if(i<0) {
				printf("ERROR: Operand value is not specified #%d: operand=\'L\'\n",line);
				rc=4;
				goto ENDPROC;
			}
			TBL.ch[cn].len=i;
			if('.'==*(cp+1)) {
				cp++;
				TBL.ch[cn].huten=1;
			} else {
				TBL.ch[cn].huten=0;
			}
		}
		else if('O'==*cp) {			/* �I�N�^�[�u */
			cp++;
			i=getval(&cp);
			if(i<0) {
				printf("ERROR: Operand value is not specified #%d: operand=\'O\'\n",line);
				rc=4;
				goto ENDPROC;
			}
			TBL.ch[cn].oct=i;
		}
		else if('>'==*cp) {			/* 1�I�N�^�[�u�グ�� */
			TBL.ch[cn].oct++;
		}
		else if('<'==*cp) {			/* 1�I�N�^�[�u������ */
			TBL.ch[cn].oct--;
		}
		else if('V'==*cp) {			/* �{�����[�� */
			cp++;
			if('+'==*cp || '-'==*cp) {
				for(;'+'==*cp || '-'==*cp;cp++) {
					if('+'==*cp) TBL.ch[cn].vol++;
					else TBL.ch[cn].vol--;
				}
				cp--;
				note.type=NTYPE_VOL;
				note.op1=(unsigned char)cn;
				note.op2=0;
				note.op3=0;
				note.val=TBL.ch[cn].vol;
				if(0!=(rc=cadd(cn,&note))) {
					goto ENDPROC;
				}
			} else {
				i=getval(&cp);
				if(i<0) {
					printf("ERROR: Operand value is not specified #%d: operand=\'V\'\n",line);
					rc=4;
					goto ENDPROC;
				}
				note.type=NTYPE_VOL;
				note.op1=(unsigned char)cn;
				note.op2=0;
				note.op3=0;
				note.val=i;
				if(0!=(rc=cadd(cn,&note))) {
					goto ENDPROC;
				}
				TBL.ch[cn].vol=i;
			}
		}
		else if('M'==*cp) {			/* �}�X�^�[�{�����[�� */
			cp++;
			i=getval(&cp);
			if(i<0) {
				printf("ERROR: Operand value is not specified #%d: operand=\'M\'\n",line);
				rc=4;
				goto ENDPROC;
			}
			note.type=NTYPE_MVOL;
			note.op1=0;
			note.op2=0;
			note.op3=0;
			note.val=i;
			if(0!=(rc=cadd(cn,&note))) {
				goto ENDPROC;
			}
		}
		else if('|'==*cp) {			/* ���x�� */
			note.type=NTYPE_LABEL;
			note.op1=0;
			note.op2=0;
			note.op3=0;
			note.val=0;
			if(0!=(rc=cadd(cn,&note))) {
				goto ENDPROC;
			}
		}
		else if('K'==*cp) {			/* key base */
			cp++;
			if('+'==*cp || '-'==*cp) {
				for(;'+'==*cp || '-'==*cp;cp++) {
					if('+'==*cp) kbase[cn]++;
					else kbase[cn]--;
				}
				cp--;
			} else {
				printf("ERROR: Operand value is not specified #%d: operand=\'K\'\n",line);
				rc=4;
				goto ENDPROC;
			}
		}
		else {						/* �s�� */
			printf("ERROR: Unknown operand #%d: operand=\'%c\'\n",line,*cp);
			rc=4;
			goto ENDPROC;
		}
	}

ENDPROC:
	free(cbuf);
	return rc;
}

/* ���l�����o�� */
static int getval(char** str)
{
	int n=0;
	char* ptr;
	for(ptr=*str;isdigit(*ptr);ptr++) {
		n*=10;
		n+=(*ptr)-'0';
	}
	if(ptr==*str) {
		return -1;
	}
	(*str)=ptr-1;
	return n;
}

/* �`���l���ʂ̃m�[�g���ǉ� */
static int cadd(int cn,struct _NOTE* note)
{
	struct Channel* ch;
	ch=(struct Channel*)malloc(sizeof(struct Channel));
	if(NULL==ch) {
		puts("ERROR: No memory.");
		return 255;
	}
	memcpy(&(ch->note),note,sizeof(struct _NOTE));
	ch->next=NULL;
	if(NULL==TBL.ch[cn].tail) {
		TBL.ch[cn].tail=ch;
		TBL.ch[cn].head=ch;
	} else {
		TBL.ch[cn].tail->next=ch;
		TBL.ch[cn].tail=ch;
	}
	return 0;
}

/* �`���l���̃}�[�W���� */
static int merge(struct Channel* c2)
{
	struct Channel* c1=TBL.ch[0].head;
	TBL.ch[6].head=NULL;
	TBL.ch[6].tail=NULL;
	while(1) {
		for(;NTYPE_WAIT!=c1->note.type;c1=c1->next) {
			if(cadd(6,&(c1->note))) {
				return -1;
			}
		}
		for(;NTYPE_WAIT!=c2->note.type;c2=c2->next) {
			if(cadd(6,&(c2->note))) {
				return -1;
			}
		}
		if(0==c1->note.val || 0==c2->note.val) {
			break;
		}
		if(c1->note.val < c2->note.val) {
			c2->note.val-=c1->note.val;
			if(cadd(6,&(c1->note))) {
				return -1;
			}
			c1=c1->next;
		}
		else if(c1->note.val > c2->note.val){
			c1->note.val-=c2->note.val;
			if(cadd(6,&(c2->note))) {
				return -1;
			}
			c2=c2->next;
		}
		else {
			if(cadd(6,&(c1->note))) {
				return -1;
			}
			c1=c1->next;
			c2=c2->next;
		}
	}
	for(;NTYPE_WAIT!=c1->note.type||0!=c1->note.val;c1=c1->next) {
		if(cadd(6,&(c1->note))) {
			return -1;
		}
	}
	for(;NTYPE_WAIT!=c2->note.type||0!=c2->note.val;c2=c2->next) {
		if(cadd(6,&(c2->note))) {
			return -1;
		}
	}
	if(cadd(6,&(c2->note))) {
		return -1;
	}
	TBL.ch[0].head=TBL.ch[6].head;
	TBL.ch[0].tail=TBL.ch[6].tail;
	TBL.ch[6].head=NULL;
	TBL.ch[6].tail=NULL;
	return 0;
}
