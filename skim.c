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
