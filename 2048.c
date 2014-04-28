#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

// Window's size
#define WW 40
#define WH 11

// Global vals
bool anim;
char emptyCell[5];
char cell[4][4][5];
unsigned short score;
bool gameEnded;

// Prototypes
void gotoXY(unsigned short,unsigned short);
void setColor(char*);
void center(char*,unsigned short);
void init();
void welcomeView();
void endView();
void display();
bool isEmpty(char*);
void addRandomValue(bool);
short mergeable(short,short);
short merge(char*,char*,bool);
unsigned short getAction();
bool doProcess(unsigned short);


void gotoXY(unsigned short x,unsigned short y){
 HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
 COORD position={x,y};
 SetConsoleCursorPosition(handle,position);
}
void setColor(char *cell1){
 unsigned short cColor;
 switch(atoi(cell1)){
  case 2: cColor=112; break;
  case 4: cColor=128; break;
  case 8: cColor=48; break;
  case 16: cColor=32; break;
  case 32: cColor=72; break;
  case 64: cColor=71; break;
  case 128: cColor=96; break;
  case 256: cColor=103; break;
  case 512: cColor=103; break;
  case 1024: cColor=160; break;
  case 2048: cColor=224; break;
  case 4096: cColor=224; break;
  default: cColor=7; break;
 }
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),cColor);
}
void center(char *str, unsigned short y){
 gotoXY((WW-strlen(str))/2,y);
 printf("%s",str);
}



void init(){
 // Game coded to learn C
 SetConsoleTitle("2048 :: YoannBoyer.com");

 HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
 SMALL_RECT windowSize = {0,0,WW,WH};
 SetConsoleWindowInfo(handle,1,&windowSize);

 CONSOLE_SCREEN_BUFFER_INFO info;
 GetConsoleScreenBufferInfo(handle,&info);
 COORD new_size = {info.srWindow.Right-info.srWindow.Left+1,info.srWindow.Bottom-info.srWindow.Top+1};
 SetConsoleScreenBufferSize(handle,new_size);

 srand(time(NULL));

 strcpy(emptyCell,"    ");

 anim = false;
 gameEnded = false;

 unsigned short x,y;
 for(x=0; x<4; x++){
  for(y=0; y<4; y++){
   strcpy(cell[x][y], emptyCell);
  }
 }

 score = 0;

 addRandomValue(true);
 addRandomValue(true);
}


void welcomeView(){
 center("2048",2);
 center("Press Enter to start",7);
 center("",WH);

 while(getch() != 13){
 }

 system("cls");
}
void endView(){
 system("cls");
 gameEnded = true;

 center("Press Enter to restart", 10);
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
 center("You lost !", 6);
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
 center("Esc to exit", 11);
 setColor("");

 display();

 gotoXY(WW,WH);

 bool checkAction = true;
 unsigned short ch;
 while(checkAction){
  ch = getch();
  if (ch == 13){
   checkAction = false;
   system("cls");
   init();
  } else if (ch == 27){
   exit(0);
  }
 }
}

void display(){
 unsigned short x,y;
 if (!gameEnded){
  for (x=0; x<4; x++){
   gotoXY((WW-20)/2,5+x);
   for(y = 0; y < 4; y++){
    printf("|");
    setColor(cell[x][y]);
    printf("%s", cell[x][y]);
    setColor("");
   }
   printf("|");
  }
 }

 char str[13] = "Score : ";
 char sc[5];
 sprintf(sc,"%d",score);
 strcat(str,sc);
 center(str,2);

 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
 gotoXY(WW,0);
 printf(" ");
 gotoXY(WW-9,0);
 printf("Anim : ");
 printf(anim == true ? "ON" : "OFF");
 setColor("");

 center("",WH);
}


bool isEmpty(char *cell1){
 return strcmp(cell1,emptyCell) ? false : true;
}

void addRandomValue(bool add){
 unsigned short x,y;

 if (add){
  do{
   x = rand()%4;
   y = rand()%4;
  } while (!isEmpty(cell[x][y]));

  char str[5];
  sprintf(str,"%4d",(rand()%2+1)*2);
  strcpy(cell[x][y], str);

  bool end = true;
  bool fullBoard = true;
  for(x=0; x<4; x++){
   for(y=0; y<4; y++){
    if (isEmpty(cell[x][y])){
     fullBoard = false;
    }
    if (mergeable(x,y) > 0){
     end = false;
    }
   }
  }
  if (end && fullBoard){
   endView();
  }
 }
}

short mergeable(short r, short c){
 unsigned short i=0;

 if (c > -1){
  if (!isEmpty(cell[0][c])){
   if (!strcmp(cell[0][c],cell[1][c])){
    i++;
   } else if (!strcmp(cell[0][c],cell[2][c]) && isEmpty(cell[1][c])){
    i++;
   } else if (!strcmp(cell[0][c],cell[3][c]) && isEmpty(cell[1][c]) && isEmpty(cell[2][c])){
    i++;
   }
  }

  if (!isEmpty(cell[1][c]) && i == 0){
   if (!strcmp(cell[1][c],cell[2][c])){
    i++;
   } else if (!strcmp(cell[1][c],cell[3][c]) && isEmpty(cell[2][c])){
    i++;
   }
  }

  if (!strcmp(cell[2][c],cell[3][c]) && !isEmpty(cell[2][c])){
   i++;
  }
 }

 if (r > -1){
  if (!isEmpty(cell[r][0])){
   if (!strcmp(cell[r][0],cell[r][1])){
    i++;
   } else if (!strcmp(cell[r][0],cell[r][2]) && isEmpty(cell[r][1])){
    i++;
   } else if (!strcmp(cell[r][0],cell[r][3]) && isEmpty(cell[r][1]) && isEmpty(cell[r][2])){
    i++;
   }
  }

  if (!isEmpty(cell[r][1]) && i == 0){
   if (!strcmp(cell[r][1],cell[r][2])){
    i++;
   } else if (!strcmp(cell[r][1],cell[r][3]) && isEmpty(cell[r][2])){
    i++;
   }
  }

  if (!strcmp(cell[r][2],cell[r][3]) && !isEmpty(cell[r][2])){
   i++;
  }
 }

 return i;
}
short merge(char *cell1, char *cell2, bool dontMerge){
 if (anim){
  clock_t wait = clock()+0.002*CLOCKS_PER_SEC;
  while(clock()<wait){}
  display();
 }

 if (isEmpty(cell2)){ //Nothing
  return -1;
 }

 if (isEmpty(cell1)){ //Move
  strcpy(cell1,cell2);
  strcpy(cell2,emptyCell);
  return 0;
 } else if (!strcmp(cell1, cell2) && !dontMerge){ //Merge
  strcpy(cell2,emptyCell);

  char str[5];
  sprintf(str,"%4d",atoi(cell1)*2);
  strcpy(cell1,str);
  score += atoi(cell1);
  return 1;
 } else {
  return -1;
 }
}

unsigned short getAction(){
 unsigned short ch;
 do{
  ch = getch();
  if (ch == 0xE0){
   switch(getch()){
    case 72: return 0; break;
    case 77: return 1; break;
    case 80: return 2; break;
    case 75: return 3; break;
   }
  } else if (ch == 97){
   anim = !anim;
   display();
  }
 } while(1);
}

bool doProcess(unsigned short direction){
 short x,y,mergeStatus,nbMerge;
 bool redo;
 bool done = false, merged = false;

 switch (direction){
  case 0:
  for(y=0; y<4; y++){
   redo = true;
   merged = false;
   nbMerge = mergeable(-1,y);
   while(redo){
    nbMerge--;
    redo = false;
    for(x=0; x<3; x++){
     mergeStatus = merge(cell[x][y],cell[x+1][y], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;

  case 1:
  for(x=0; x<4; x++){
   redo = true;
   merged = false;
   nbMerge = mergeable(x,-1);
   while(redo){
    nbMerge--;
    redo = false;
    for(y=3; y>0; y--){
     mergeStatus = merge(cell[x][y],cell[x][y-1], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;

  case 2:
  for(y=0; y<4; y++){
   redo = true;
   merged = false;
   nbMerge = mergeable(-1,y);
   while(redo){
    nbMerge--;
    redo = false;
    for(x = 3; x>0; x--){
     mergeStatus = merge(cell[x][y],cell[x-1][y], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;

  case 3:
  for(x=0; x<4; x++){
   redo = true;
   merged = false;
   nbMerge = mergeable(x,-1);
   while(redo){
    nbMerge--;
    redo = false;
    for(y=0; y < 3; y++){
     mergeStatus = merge(cell[x][y],cell[x][y+1], merged);
     if (mergeStatus > -1){done = true;}
     if (mergeStatus == 0){redo = true;}
     if (mergeStatus == 1){merged = true;}
    }
    if (nbMerge > 0){
     redo=true;
     merged = false;
    }
   }
  }
  break;
 }

 return done;
}


int main(){

 init();

 welcomeView();

 while(1){
  display();
  addRandomValue(doProcess(getAction()));
 }
 return 0;
}
