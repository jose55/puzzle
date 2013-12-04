//#include "square.h"

#include <allegro.h>
#include <stdlib.h>//for rand()
#include <time.h>//for srand()
#include <string>


//BITMAP * images[9];//pointer to the image of each square
int numberImage[3][3];//this is the grid which contains the current number of image in each square
//numbers from 0 to 8


class square
{
  static bool checkOpossites(int);
public:
  static char* initial_random;
  static int initial_movements;// index of initial_random up to level
  static bool something_moving;
private:
  static int xAbsolute,yAbsolute;
  int x,y;//relatives to the former one
  int aim;////the coordenates we are going to reach
  int node;
  int aimNode;
  int image;//image which is holding this piece
  bool empty;//only one is empty
  int state;//0 quite, 1 moving right  2 going down  3 going left   4 going up
  void moveRight(square *[],bool);
  void moveLeft(square *[],bool);
  void moveUp(square *[]);
  void moveDown(square *[]);
  void drawInBuffer(bool);
  bool isPossibleStartToTheRight(bool);
  bool isPossibleStartToTheLeft(bool);
  bool isPossibleStartUpwards();
  bool isPossibleStartDownwards();
  static void checkIntroduction(){ if(initial_movements<level)initial_movements++;
    else  introduction=false;}


public:
  static bool introduction;
  static int level;
  static void Random();  
  int index;//the number of the image belonging to the present object (square)
  square(const int);//node is the parameter... this constructor assigns random images
  square();//this constructor is to put all the images in order
  static  void InitFewRandomMovements(square * [],int);
  void move(square *[]);
  bool check();

};


int square::xAbsolute=250;//absolute coordenates of the first square (up-left)
int square::yAbsolute=50;
bool square::something_moving=false;
int square::level=1;// we move ony 1 
int square::initial_movements=0;
char* square::initial_random=NULL;
bool square::introduction=true;

//**** GLOBAL FUNCTIONS ****
void initializeAllegro();
void createBuffers();
void loadImages();
void main_loop();
void initializeRand();
bool wasItAlready(const int,const int);
void initializeBuffers();
//**** END GLOBAL FUNCTIONS *******


//**** GLOBAL VARIABLES ****
BITMAP *  buffer;
BITMAP * imageBmp[9];
bool done=false;

//***** END GLOBAL VARIABLES *****

int main()
{
  
  initializeRand();

  initializeAllegro();

  initializeBuffers();

  loadImages();
   do{
    main_loop();
    square::level++;
    square::introduction=true;
    square::initial_movements=0;
    //square::initial_random=NULL;
    square::something_moving=false;
    done=false;
    while(!key[KEY_ENTER] && !key[KEY_ESC]){};
   }
    while(!key[KEY_ESC]);

    

  allegro_exit();

  return 0;
}

void initializeRand()
{
  //we'll get actual random numbers not pseudo ones.
  srand(time(NULL));
}


void initializeAllegro()
{
  allegro_init();
  install_keyboard();
  set_color_depth(16);
  set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,1200,600,0,0);

}
void initializeBuffers()
{
  buffer=create_bitmap(800,500);
}
		       
void loadImages()
{
 
  imageBmp[0]=load_bitmap("cullera0.bmp",NULL);
  imageBmp[1]=load_bitmap("cullera1.bmp",NULL);
  imageBmp[2]=load_bitmap("cullera2.bmp",NULL);
  imageBmp[3]=load_bitmap("cullera3.bmp",NULL);
  imageBmp[4]=load_bitmap("cullera4.bmp",NULL);
  imageBmp[5]=load_bitmap("cullera5.bmp",NULL);
  imageBmp[6]=load_bitmap("cullera6.bmp",NULL);
  imageBmp[7]=load_bitmap("cullera7.bmp",NULL);
  imageBmp[8]=load_bitmap("cullera8.bmp",NULL);
  
}

void main_loop()
{
  bool game=true;

  //assign a random image 
  square piece[9]={0,1,2,3,4,5,6,7,8};
  //  int *a[9];
  square *pPiece[9];
  for(int x=0;x<9;x++)
    pPiece[x]=&piece[x];// ** pPiece is an array of pointers to square **

  //  square::InitFewRandomMovements(pPiece,square::level);
 
  square::Random();
  char text[10];
  while(game)
    {
      clear_to_color(buffer,0);

      //it goes through all pieces to control their movements
      for(int p=0;p<9;p++)piece[p].move(pPiece);//the parametre should be an array of pointer to every piece

        
      //check if every pice is in its place
      //the number of the nodes needs to amtch the images' number
      if(!square::introduction)
	{
	  for(int x=0;x<9;x++)
	    {
	      if(piece[x].check())done=true;
	      else {done=false;break;}
	      
	    }
	}
      textout_ex(buffer, font, "PUZZLE .... BY JOSE MENDEZ REQUENA", 350, 20, makecol(255,255,255),
makecol(0, 0, 0));

      switch(square::level)
	{
	case 1:
	  strcpy(text,"level 1");
	  break;
	case 2:
	  strcpy(text,"level 2");
	  break;
	case 3:
	  strcpy(text,"level 3");
	  break;
	case 4:
	  strcpy(text,"level 4");
	  break;
	case 5:
	  strcpy(text,"level 5");
	  break;
	case 6:
	  strcpy(text,"level 6");
	  break;
	case 7:
	  strcpy(text,"level 7");
	  break;
	case 8:
	  strcpy(text,"level 8");
	  break;
	case 10:
	  strcpy(text,"level 10");
	  break;
	case 11:
	  strcpy(text,"level 11");
	  break;
	case 12:
	  strcpy(text,"level 12");
	  break;
	case 13:
	  strcpy(text,"level 13");
	  break;
	}

	

      textout_ex(buffer, font, text, 500, 40, makecol(255,255,255),
makecol(0, 0, 0));

      draw_sprite(screen,buffer,100,0);

      if(key[KEY_ESC] || done)game=false; 
      else(game=true);
    }

  delete square::initial_random;
}

bool wasItAlready(const int current_square,const int random_value)
{

  for(int x=0;x<current_square;x++)
    if(numberImage[x % 3][x / 3]==random_value)return true;

  return false;
}

square::square(const int number_square)//no number of image
{
  //
  //assign an random image;
  //
  state=0;//quite

  do{
    image=rand() % 9;//store the random number in the instance variable
  }                   //from 0 to 8 ;
  while(wasItAlready(number_square,image));

  image=number_square;

  numberImage[number_square % 3][number_square / 3]=number_square==4? 10:image;//store the randonm number in the grid(global variable)

  

  x=(number_square % 3) * 150;
  y=(number_square / 3) * 150;

  node=number_square;

  empty=number_square==4 ? true:false;//In the biginning all of them 


  drawInBuffer(empty);
  

}

square::square()
{
  state=0;


}




void square::drawInBuffer(bool is_empty)
{
  //By now, the buffer is supossed to be clear
  //x,y  image[image]???
  if(!is_empty)draw_sprite(buffer,imageBmp[image],xAbsolute+x,yAbsolute+y);
  
}

bool square::isPossibleStartToTheRight(bool inforce)
{

  return (key[KEY_RIGHT] && !state && !(node==2 || node==5 || node== 8) && numberImage[(node % 3)+1][node /3]==10 && !something_moving) ||  (numberImage[(node % 3)+1][node /3]==10 && *(initial_random+initial_movements)=='r' && !something_moving && square::introduction);
}

void square::moveRight(square *pointerToPieces[],bool inforce=false)
{
  if(isPossibleStartToTheRight(inforce) )
    {
      //start moving ...
      square::something_moving=true;
      state=1;
      aim=x+150;
      aimNode++;

      checkIntroduction();
	
    }
  if(state==1)
    {
      //it is already moving towards right 
      x+=10;
      if(x>aim)
	{
		  state=0;//quiet
		  square::something_moving=false;
		  x=aim-150;//restablishes x to its possition

		  //we swap the new empty
		  empty=true;
		  pointerToPieces[node+1]->empty=false;	

		  //now we have to exchange (swap) the images (the instance variable)
		  int imageAux=image;
		  image=pointerToPieces[node+1]->image;
		  pointerToPieces[node+1]->image=imageAux;

		  //now we swap values int the grid (numberImage[3][3]) the global array
		  int aux=numberImage[node%3][node/3];
		  numberImage[node%3][node/3]=numberImage[(node+1)%3][(node+1)/3];
		  numberImage[(node+1) % 3][(node+1) / 3]=aux;
	}

    }
    drawInBuffer(empty);
  
}

bool square::isPossibleStartToTheLeft(bool inforce)
{

  return (key[KEY_LEFT] && !state && !(node==0 || node==3 || node==6) && numberImage[(node % 3)-1][node /3]==10 && !something_moving) || (numberImage[(node % 3)-1][node /3]==10 && *(initial_random+initial_movements)=='l' && !something_moving && square::introduction);
}
void square::moveLeft(square *pointerToPieces[], bool inforce=false)
{
if(isPossibleStartToTheLeft(inforce) )
    {
      //start moving ...
      square::something_moving=true;
      state=3;
      aim=x-150;
      aimNode--;
      checkIntroduction();

    }
  if(state==3)
    {
      //it is already moving towards left
      x-=10;
      if(x<aim)
	{
		  state=0;//quiet
		  square::something_moving=false;
		  x=aim+150;//restablishes x to its possition

		  //we swap the new empty
		  empty=true;
		  pointerToPieces[node-1]->empty=false;	

		  //now we have to exchange (swap) the images (the instance variable)
		  int imageAux=image;
		  image=pointerToPieces[node-1]->image;
		  pointerToPieces[node-1]->image=imageAux;

		  //now we swap values int the grid (numberImage[3][3]) the global array
		  int aux=numberImage[node%3][node/3];
		  numberImage[node%3][node/3]=numberImage[(node-1)%3][(node-1)/3];
		  numberImage[(node-1) % 3][(node-1) / 3]=aux;
	}
      drawInBuffer(empty);
      
    }
  
}
bool square::isPossibleStartDownwards()
{

  return (key[KEY_DOWN] && !state && !(node==6 || node==7 || node==8) && numberImage[(node % 3)][(node /3)+1]==10 && !something_moving) || (numberImage[node % 3][(node /3)+1]==10 && *(initial_random+initial_movements)=='d' && !something_moving && square::introduction);
}
void square::moveDown(square *pointerToPieces[])
{
if(isPossibleStartDownwards() )
    {
      //start moving ...
      square::something_moving=true;
      state=2;
      aim=y+150;
      aimNode+=3;
      checkIntroduction();
    }
  if(state==2)
    {
      //it is already moving towards left
      y+=10;
      if(y>aim)
	{
		  state=0;//quiet
		  square::something_moving=false;
		  y=aim-150;//restablishes x to its possition

		  //we swap the new empty
		  empty=true;
		  pointerToPieces[node+3]->empty=false;	

		  //now we have to exchange (swap) the images (the instance variable)
		  int imageAux=image;
		  image=pointerToPieces[node+3]->image;
		  pointerToPieces[node+3]->image=imageAux;

		  //now we swap values int the grid (numberImage[3][3]) the global array
		  int aux=numberImage[node%3][node/3];
		  numberImage[node%3][node/3]=numberImage[(node+3)%3][(node+3)/3];
		  numberImage[(node+3) % 3][(node+3) / 3]=aux;
	}
      drawInBuffer(empty);
      
    }
  
}

bool square::isPossibleStartUpwards()
{

  return (key[KEY_UP] && !state && !(node==0 || node==1 || node==2) && numberImage[(node % 3)][(node /3)-1]==10 && !something_moving) || (numberImage[node % 3][(node /3)-1]==10 && *(initial_random+initial_movements)=='u' && !something_moving && square::introduction);
}
void square::moveUp(square *pointerToPieces[])
{
if(isPossibleStartUpwards() )
    {
      //start moving ...
      square::something_moving=true;
      state=4;
      aim=y-150;
      aimNode-=3;
      checkIntroduction();
    }
  if(state==4)
    {
      //it is already moving towards left
      y-=10;
      if(y<aim)
	{
		  state=0;//quiet
		  square::something_moving=false;
		  y=aim+150;//restablishes x to its possition

		  //we swap the new empty
		  empty=true;
		  pointerToPieces[node-3]->empty=false;	

		  //now we have to exchange (swap) the images (the instance variable)
		  int imageAux=image;
		  image=pointerToPieces[node-3]->image;
		  pointerToPieces[node-3]->image=imageAux;

		  //now we swap values int the grid (numberImage[3][3]) the global array
		  int aux=numberImage[node%3][node/3];
		  numberImage[node%3][node/3]=numberImage[(node-3)%3][(node-3)/3];
		  numberImage[(node-3) % 3][(node-3) / 3]=aux;
	}
      drawInBuffer(empty);
      
    }
  
}

void square::move(square *pointerToPieces[])
{

  moveRight(pointerToPieces);
  moveLeft(pointerToPieces);
  moveUp(pointerToPieces);
  moveDown(pointerToPieces);

}

bool square::check()
{

  return node==image;
}

void square::InitFewRandomMovements(square * pointer_pieces  [],int ammount_pieces)
{
  // *************** pointer_pieces is the name of an array of poiners to square objects *******************************
  //level 1->we move 1 piece
  //level 2->we move 2 pieces
  //so on...

  int *pointer_to_random_numbers=new int[ammount_pieces];

  //we fill the array with random numbers
  for(int x=0;x<ammount_pieces;x++)
    {
      *(pointer_to_random_numbers+x)=rand() % 4;
    }  


  for(int x=0;x<9;x++)pointer_pieces[x]->moveRight(pointer_pieces,true); 
  for(int x=0;x<9;x++)pointer_pieces[x]->moveLeft(pointer_pieces,true); 

}

void square::Random()
{

  square::initial_random=new char[square::level];

  int node_empty=4;//we start with the node 4 empty

  for(int x=0;x<square::level;x++)
    {
      int n;
      if(node_empty==4)
	{//it's the central piece, any movement is ok
	  do{
	  n=rand() % 4 + 1;
	  *(square::initial_random+x)= n==1? 'r':n==2? 'd':n==3? 'l':'u'; 
	  }
	  while(checkOpossites(x) && x);
	  switch(n)
	    {
	    case 1:
	      node_empty--;
	      break;
	    case 2:
	      node_empty-=3;
	      break;
	    case 3:
	      node_empty++;
	      break;
	    case 4:
	      node_empty+=3;
	      break;
	      
	    }

	}//end if
      
  
      else if(node_empty==1)
	{
	  do{
	    n=rand() % 3;
	  
	  *(square::initial_random+x)= n==0? 'r':n==1? 'l':'u'; 
	  }
	  while(checkOpossites(x));
	  
	  switch(n)
	    {
	    case 0:
	      node_empty--;
	      break;
	    case 1:
	      node_empty++;
	      break;
	    default://case 2
	      node_empty+=3;
	      break;
	    }
	  
	}//end if
      
      
      else if(node_empty==7)
	{
	  do{
	    n=rand() % 3;
	    *(square::initial_random+x)= n==0? 'r':n==1? 'l':'d'; 
	  }
	  while(checkOpossites(x));

	  switch(n)
	    {
	    case 0:
	      node_empty--;
	      break;
	    case 1:
	      node_empty++;
	      break;
	    default://case 2
	      node_empty-=3;
	      break;
	    }
	  
	}//end if
      
      else if(node_empty==3)
	{
	  do{
	    n=rand() % 3;
	    *(square::initial_random+x)= n==0? 'l':n==1? 'u':'d'; 
	  }
	  while(checkOpossites(x));

	  switch(n)
	    {
	    case 0:
	      node_empty++;
	      break;
	    case 1:
	      node_empty+=3;
	      break;
	    default://case 2
	      node_empty-=3;
	      break;
	    }
	  
	}//end if
      
      else if(node_empty==5)
	{
	  do{
	    n=rand() % 3;
	    *(square::initial_random+x)= n==0? 'r':n==1? 'u':'d'; 
	  }
	  while(checkOpossites(x));

	  switch(n)
	    {
	    case 0:
	      node_empty--;
	      break;
	    case 1:
	      node_empty+=3;
	      break;
	    default://case 2
	      node_empty-=3;
	      break;
	    }
	  
	}//end if

      else if(node_empty==0)
	{
	  do{
	  n=rand() % 2;
	  *(square::initial_random+x)= n==0? 'u':'l'; 
	  }
	  while(checkOpossites(x));
	  
	  switch(n)
	    {
	    case 0:
	      node_empty+=3;
	      break;
	    default:
	      node_empty++;
	      break;
	    }
	  
	}//end if

      else if(node_empty==2)
	{
	  do{
	  
	    n=rand() % 2;
	    
	    *(square::initial_random+x)= n==0? 'u':'r'; 
	  }
	  while(checkOpossites(x));

	  switch(n)
	    {
	    case 0:
	      node_empty+=3;
	      break;
	    default:
	      node_empty--;
	      break;
	    }
	  
	}//end if

      else if(node_empty==6)
	{
	  do{
	  n=rand() % 2;
	  
	  *(square::initial_random+x)= n==0? 'd':'l'; 
	  }
	  while(checkOpossites(x));

	  switch(n)
	    {
	    case 0:
	      node_empty-=3;
	      break;
	    default:
	      node_empty++;
	      break;
	    }
	  
	}//end if

      else //node_empty should be 8
	{

	  do{
	    n=rand() % 2;
	    *(square::initial_random+x)= n==0? 'd':'r'; 
	  }
	  while(checkOpossites(x));
	  switch(n)
	    {
	    case 0:
	      node_empty-=3;
	      break;
	    default:
	      node_empty--;
	      break;
	    }
	  
	}//end if
    }//end for
}//enf function


bool square::checkOpossites(int index)
{
  return (*(initial_random+(index-1))=='u' && *(initial_random+index)=='d') || (*(initial_random+(index-1))=='r' && *(initial_random+index)=='l') || (  *(initial_random+(index-1))=='d' && *(initial_random+index)=='u'  ) || (*(initial_random+(index-1))=='l' && *(initial_random+index)=='r'    );
}
