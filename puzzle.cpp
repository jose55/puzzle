#include <allegro.h>
#include <stdlib.h>//for rand()
#include <time.h>//for srand()
//BITMAP * images[9];//pointer to the image of each square
int numberImage[3][3];//this is the grid which contains the current number of image in each square
//numbers from 0 to 8


class square
{
  static bool something_moving;
  static int xAbsolute,yAbsolute;
  int x,y;//relatives to the former one
  int aim;////the coordenates we are going to reach
  int node;
  int aimNode;
 int image;//image which is holding this piece
  bool empty;//only one is empty
  int state;//0 quite, 1 moving right  2 going down  3 going left   4 going up
  void moveRight(square *[]);
  void moveLeft(square *[]);
  void moveUp(square *[]);
  void moveDown(square *[]);
  void drawInBuffer(bool);
  bool isPossibleStartToTheRight();
  bool isPossibleStartToTheLeft();
  bool isPossibleStartUpwards();
  bool isPossibleStartDownwards();

public:
  
  int index;//the number of the image belonging to the present object (square)
  square(const int);//node is the parameter
  void move(square *[]);
};

int square::xAbsolute=100;//absolute coordenates of the first square (up-left)
int square::yAbsolute=100;
bool square::something_moving=false;


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

//***** END GLOBAL VARIABLES *****

int main()
{
  
  initializeRand();

  initializeAllegro();

  initializeBuffers();

  loadImages();

  main_loop();

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
  set_gfx_mode(GFX_AUTODETECT_WINDOWED,1200,600,0,0);

}
void initializeBuffers()
{
  buffer=create_bitmap(800,500);
}
		       
void loadImages()
{
 
  imageBmp[0]=load_bitmap("puzzle0.bmp",NULL);
  imageBmp[1]=load_bitmap("puzzle1.bmp",NULL);
  imageBmp[2]=load_bitmap("puzzle2.bmp",NULL);
  imageBmp[3]=load_bitmap("puzzle3.bmp",NULL);
  imageBmp[4]=load_bitmap("puzzle4.bmp",NULL);
  imageBmp[5]=load_bitmap("puzzle5.bmp",NULL);
  imageBmp[6]=load_bitmap("puzzle6.bmp",NULL);
  imageBmp[7]=load_bitmap("puzzle7.bmp",NULL);
  imageBmp[8]=load_bitmap("puzzle8.bmp",NULL);
  
}

void main_loop()
{
  bool game=true;

  //assign a random image 
  square piece[9]={0,1,2,3,4,5,6,7,8};
  //  int *a[9];
  square *pPiece[9];
  for(int x=0;x<9;x++)
    pPiece[x]=&piece[x];

  while(game)
    {
      clear_to_color(buffer,0);
      int n=5;
      for(int p=0;p<9;p++)piece[p].move(pPiece);//the parametre should be an array of pointer to every piece

	
      
      draw_sprite(screen,buffer,100,0);

      if(key[KEY_ESC])game=false; 
    }


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

  numberImage[number_square % 3][number_square / 3]=number_square==4? 10:image;//store the randonm number in the grid(global variable)
  //
  x=(number_square % 3) * 150;
  y=(number_square / 3) * 150;

  node=number_square;

  empty=number_square==4 ? true:false;//In the biginning all of them 


  drawInBuffer(empty);
  

}
void square::drawInBuffer(bool is_empty)
{
  //By now, the buffer is supossed to be clear
  //x,y  image[image]???
  if(!is_empty)draw_sprite(buffer,imageBmp[image],xAbsolute+x,yAbsolute+y);
  
}

bool square::isPossibleStartToTheRight()
{

  return key[KEY_RIGHT] && !state && !(node==2 || node==5 || node== 8) && numberImage[(node % 3)+1][node /3]==10 && !something_moving;
}

void square::moveRight(square *pointerToPieces[])
{
  if(isPossibleStartToTheRight() )
    {
      //start moving ...
      square::something_moving=true;
      state=1;
      aim=x+150;
      aimNode++;
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

bool square::isPossibleStartToTheLeft()
{

  return key[KEY_LEFT] && !state && !(node==0 || node==3 || node==6) && numberImage[(node % 3)-1][node /3]==10 && !something_moving;
}
void square::moveLeft(square *pointerToPieces[])
{
if(isPossibleStartToTheLeft() )
    {
      //start moving ...
      square::something_moving=true;
      state=3;
      aim=x-150;
      aimNode--;
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

  return key[KEY_DOWN] && !state && !(node==6 || node==7 || node==8) && numberImage[(node % 3)][(node /3)+1]==10 && !something_moving;
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

  return key[KEY_UP] && !state && !(node==0 || node==1 || node==2) && numberImage[(node % 3)][(node /3)-1]==10 && !something_moving;
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
