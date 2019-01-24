//各種仕様
/*
ひとつのファイルにつき保存できる単語数は100まで
日本語及び英語の文字列長は30まで
ファイル名は１５文字まで
filewrite();でfilename[0]にデータ保存。返り値なし
fileread();でfilename[0]からデータ取り出し。失敗時1、成功時0の返り値
*/

//色の設定
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define RESET   "\033[0m"

#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>



//World変数の宣言
char *message ="エラーが発生しました";
int memorize[100];//暗記できたら1初期状態は0
int mistake[100];//間違えた回数
char jap[100][30];//日本語
char eng[100][30];//英語
int last[100];//前回の答案結果。正解していれば1初期状態0
int total;//保存されている単語数
int unuse;//初期状態0。一度でもといたことのあるファイルの場合１
char filename[1][30];//ファイル名
int success=0;//正解数

//出力に関する関数宣言
void errorp(char a[200]);//赤色
void infop(char a[200]);//青色
void okp(char a[200]);//緑
void questionp(char a[200]);//黄色(改行なし)
void optionp(char a[200]);//白色

//関数の宣言
void boot();//初回起動時
void mainmanu();//動作選択
void skim();//一覧表示
void words();//単語帳
void create();//問題新規作成
void solve();//問題を解く
void weak();//苦手
void change();//問題の変更
void debug();//変数内容確認（完成時削除予定
void filewrite();//変数保存関数
int filecheck();//ファイルの有無確認
int fileread();//変数読み出し関数
void ending();//プログラム終了時の動作



int main(){
	boot();
	mainmanu();
	return 0;
}

void boot(){
	char *dir ="./Databases";
	struct stat statBuf;
	
	if(stat(dir,&statBuf)!=0){//Databasesファイルが存在するかどうかの確認
		mkdir(dir,0755);//存在しなければ作成
	}
}

void mainmanu(){
	char input[100];
	int num;
	int len;
	int gomi;//完成時削除
	while(1){
		//system("clear");
		optionp("1.一覧表示");
		optionp("2.単語帳");
		optionp("3.問題の作成");
		optionp("4.問題を解く");
		optionp("5.苦手");
		optionp("6.問題の変更");
		optionp("0.終了");
		questionp("モード番号を入力してください");
		scanf("%s",input);
		len=strlen(input);
		if(len!=1){
			num=9;	
		}else if(isdigit(input[0])){
			num=atoi(input);
		}else{
			num=9;
		}
		
		switch(num){
			case 1:
				skim();
				break;
			case 2:
				words();
				break;
			case 3:
				create();
				break;
			case 4:
				solve();
				break;
			case 5:
				weak();
				break;
			case 6:
				change();
				break;
			case 7://完成時削除
				debug();
				break;
			case 8://完成時削除
				strcpy(filename[0],"./Databases/L1-1");
				gomi=fileread();
				break;
			case 9:
				system("clear");
				break;
			case 0:
				infop("Good Bye!!");
				ending();
				return;
			default:
				errorp("存在しないモードが選択されました。");
		}
	}
}

void filewrite(){
	int i;
	FILE *fo;
	fo=fopen(filename[0],"w");
	fprintf(fo,"%d,%d\n",total,unuse);
	for(i=0;i<total;i++){
		fprintf(fo,"%d,%d,%s,%d,%s\n",memorize[i],mistake[i],eng[i],last[i],jap[i]);
	}
	fclose(fo);
	okp("FILE WRITE COMPLETE");
}

int fileread(){
	int i;
		FILE *FI = fopen(filename[0], "r");
    		if (FI == NULL) {
    			errorp("存在しないデータファイルが入力されました。");
        		return 1;
    		}
		
		fscanf(FI,"%d,%d",&total,&unuse);
		for(i=0;i<total;i++){
			fscanf(FI,"%d,%d,%[^,],%d,%s",&memorize[i],&mistake[i],eng[i],&last[i],jap[i]);
		}
			
    		fclose(FI);
    		okp("FILE READ COMPLETED");
    		return 0;
}

void skim(){
	int i,j,len;
	char a[1][15];
	for(;;){
		questionp("表示したい問題のファイル名を入力してください。");
		scanf("%s",a[0]);
		len=strlen(a[0]);
		if(len>15){
			errorp("ファイル名が長すぎます!");
			return;
		}
		strcpy(filename[0],"./Databases/");
		strcat(filename[0],a[0]);
		j=fileread();
		if(j==0) break;
	}
	infop("---------------------------------------------------\n");
	infop("|番号|失敗回数|    英語    |  日本語訳  |前回の結果|\n");
	for(i=0;i<total;i++){
		printf("%d,%d,%s,%s,%d\n",memorize[i],mistake[i],eng[i],jap[i],last[i]);
	}
	infop("---------------------------------------------------\n");
	printf("総単語数：%d\n",total);
	printf("unuse=%d\n",unuse);
	okp("一覧出力完了.\n");
}

void words(){
	int kana,c,i,f;
	char n[9];
	printf("単語帳を開始します。\n");
	printf("開始：１　終了：２\n");
	scanf("%s",n);
	kana=atoi(n);
	f=0;
	while(f==0){
		if(kana==0){
			printf("正しく数字で入力してください\n");	
			return;	
		}
		else if(kana==1){
				for(i=0; i < total; i++){
				while(getchar() != '\n'){
				while(getchar() != '\n') ;
				}
					printf("%s\n",jap[i]);
						while(getchar() != '\n'){
						while(getchar() != '\n') ;
					}
						printf("%s\n",eng[i]);
						printf("--------------\n");				
				}
				f=1;
		}else if(kana==2){
			okp("終了します\n");
			return;
		}
	}
}

void create(){
	char a[1][15];
	int len,i;
		questionp("保存するファイル名を入力");
		scanf("%s",a[0]);
		len=strlen(a[0]);
		if(len>15){
			errorp("ファイル名が長過ぎます");
			return;
		}
		strcpy(filename[0],"./Databases/");
		strcat(filename[0],a[0]);
		if(filecheck()==1){
			errorp("すでに存在するファイルのため作成できませんでした");
			return;
		}
		questionp("作成する単語数(数値で入力)");
		scanf("%d",&total);
		if(total>100){
			errorp("単語数が大きすぎます。（最大数100）");
			return;
		}
		for(i=0;i<total;i++){
			printf(BOLDYELLOW"%d"RESET,i);
			questionp("番目の日本語を入力");
			scanf("%s",jap[i]);
			printf(BOLDYELLOW"%d"RESET,i);
			questionp("番目の英語を入力");
			scanf("%s",eng[i]);
			memorize[i]=0;
			mistake[i]=0;
			last[i]=0;
		}
		unuse=0;
		system("clear");
		okp("入力完了");
		filewrite();
		return;
	
		
}

int filecheck(){
	FILE* fp = fopen(filename[0], "r");
    if (fp == NULL) {
        return 0;
    }

    fclose(fp);
    return 1;
}


void solve(){
	int i,j,len;
	long long int rslt;
	char ans[1][30];
	char a[1][15];
	for(;;){
		printf("解きたい問題のファイル名を入力してください\n");
		scanf("%s",a[0]);
		len=strlen(a[0]);
		if(len>15){
			errorp("ファイル名が長過ぎます");
			return;
		}
		strcpy(filename[0],"./Databases/");
		strcat(filename[0],a[0]);
		j=fileread();
		if(j==0) break;
	}
	system("clear");
	for(i=0;i<total;i++){
		printf("%s\n→ ",jap[i]);
		scanf("%s",ans[0]);
		rslt=strcmp(eng[i],ans[0]);
		if(rslt==0){
			printf("正解！\n\n");
			memorize[i]=1;
			last[i]=1;
			success++;
		}else{
			printf("不正解！(%s)\n\n",eng[i]);
			mistake[i]++;
			last[i]=0;
		}
	}
	filewrite();		
}

void weak(){
	int i,j,len;
	long long int rslt;
	char ans[1][30];
	char a[1][15];
	for(;;){
		printf("解きたい問題のファイル名を入力してください\n");
		scanf("%s",a[0]);
		len=strlen(a[0]);
		if(len>15){
			errorp("ファイル名が長過ぎます");
			return;
		}
		strcpy(filename[0],"./Databases/");
		strcat(filename[0],a[0]);
		j=fileread();
		if(j==0) break;
	}
	system("clear");
	for(i=0;i<total;i++){
		if(memorize[i]==0){
			printf("%s\n→ ",jap[i]);
			scanf("%s",ans[0]);
			rslt=strcmp(eng[i],ans[0]);
			if(rslt==0){
				printf("正解！\n\n");
				memorize[i]=1;
				last[i]=1;
				success++;
			}else{
				printf("不正解！(%s)\n\n",eng[i]);
				mistake[i]++;
				last[i]=0;
			}
		}
	}
	filewrite();
	
}
void skimn(){
	int i;
	for(i=0;i < total;i++){
		printf("%d %s %s\n",i+1,jap[i],eng[i]);
	}
}

void change(){
int nj,i,lenj,lene,jnf,jcf,enf,ecf,end,kana,kanaj,kanae;
char j[1][30],e[1][30],w[9],n[9];
	printf("単語の変更\n");
	for(i = 0; i < total; i++){
		printf("%d  %s %s\n",i+1,jap[i],eng[i]);
	}
	end=0;
	while(end == 0){
		printf("日本語を変更：1　英語を変更：2 変更を終了：3\n");
		scanf("%s",w);
		kana=atoi(w);
		if (kana == 1){
			jnf=0;
			jcf=0;
			enf=0;
			ecf=0;
			while(jnf==0){
				printf("変更したい日本語の番号を入力してください\n"); 
				scanf("%s",n);
				kanaj=atoi(n);
				if(kanaj==0){
					printf("打ちなおしてください	\n");
				}
				else if (total < kanaj ){
					printf("登録された単語数を超えています\n");
					printf("もう一度入力してください\n");
				}else{
					jnf=1;
				}
			}
			while(jcf==0){
				printf("変更後の日本語を入力してください\n");
				scanf("%s",j[0]);
				lenj = strlen(j[0]);
				if (lenj > 30){
					printf("登録された日本語の文字数が多すぎます\n");
				}else{
					jcf=1;
				}
			}
			strcpy(jap[kanaj-1],j[0]);
			skimn();
			printf("正常に変更できました");
			break;
		} else if (kana == 2){
			enf=0;
			 while(enf==0){
			  	printf("変更したい英語の番号を入力してください\n");
				scanf("%s",n);
				kanae=atoi(n);
				if (kanae==0){
					printf("打ちなおしてください\n");
				}
				else if (total < kanae ){
					printf("登録されている単語数を超えています\n");
					printf("もう一度入力してください\n");
				}else{
					enf=1;	
				}
			}
			 while(ecf==0){
			 	printf("変更後の英語を入力してください\n");
				scanf("%s",e[0]);
				lene = strlen(e[0]);
				if (lene > 30){
					printf("登録された日本語の文字数が多すぎます\n");
					printf("もう一度入力してください\n");
				}else{
					ecf=1;
				}
			}
			strcpy(eng[kanae-1],e[0]);
			skimn();
			printf("正常に変更できました\n");
			}else if (kana==3){
				printf("単語の変更を終了します\n");
				end=1;
			}else if (kana==0){
				printf("正しく値を入力してください\n");
			}
			
	}
	filewrite();
}

void debug(){
	int i;
	infop("-----Debug Mode!!-----");
	printf("単語数は%d\n",total);
	printf("unuse=%d\n",unuse);
	printf("memorize[i],mistake[i],jap[i],eng[i],last[i]\n");
	for(i=0;i<total;i++){
		printf("%d,%d,%s,%s,%d\n",memorize[i],mistake[i],jap[i],eng[i],last[i]);
	}
	printf("%s\n",jap[0]);
	okp("-----OUTPUT COMPLETE-----");
}



void errorp(char a[200]){
	printf(BOLDRED"%s\n"RESET,a);
}

void infop(char a[200]){
	printf(BOLDBLUE"%s\n"RESET,a);
}

void okp(char a[200]){
	printf(BOLDGREEN"%s\n"RESET,a);
}

void questionp(char a[200]){
	printf(BOLDYELLOW"%s:"RESET,a);
}

void optionp(char a[200]){
	printf(BOLDWHITE"%s\n"RESET,a);
}

void ending(){
	int data[12];
	int q,max,trash;
	char *plotfile="./gnu.csv";
	
	max=0;
	FILE* fg=fopen(plotfile,"r");
	if(fg!=NULL){
		for(q=0;q<10;q++){
			fscanf(fg,"%d %d",&trash,&data[q]);
		}
		fclose(fg);
	}else{
		if(success==0)return;
		for(q=0;q<11;q++){
			data[q]=0;
		}
	}
	data[10]=success;
	data[11]=0;
	
	FILE* FE=fopen(plotfile,"w");
	for(q=1;q<12;q++){
		fprintf(FE,"%d %d\n",q,data[q]);
		if(data[q]>max){
			max=data[q];
		}
	}
	fclose(FE);
	FILE* gp=popen("gnuplot -persist","w");
	fprintf(gp,"set style fill solid border lc rgb \"black\"\n");
	fprintf(gp,"set xrange[0:11]\n");
		fprintf(gp,"set yrange[0:%d]\n",max+1);
		fprintf(gp,"set xlabel 'oldest........................latest'\n");
		fprintf(gp,"set ylabel 'memorized word'\n");
		fprintf(gp,"plot \"gnu.csv\" using 1:2 with boxes lw 2 lc rgb \"light-cyan\" notitle\n");
pclose(gp);
}

