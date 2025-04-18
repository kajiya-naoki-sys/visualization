#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define KEY_ESC 27
#define M_PI 3.14159265358979323846264338327950288

int count = 0;
int flag;

float lookfromx = 0.0;  /*視点のｘ座標*/
float lookfromy = 0.0;  /*視点のy座標*/
float lookfromz = 1.75; /*視点のz座標*/
float lookatx = 0.0;    /*視線のx座標*/
float lookaty = 0.5;    /*視線のy座標*/
float lookatz = 1.6;    /*視線のz座標*/
float thetaXY = 90;

float mobx = 0.;
float moby = -3.;
float mobz = 0.5;

float ground_max_x = 12;
float ground_max_y = 12;

float map_max_x = 51.0; /*マップのｘ軸最大座標*/
float map_max_y = 51.0; /*マップのｙ軸最大座標*/
int ground[101][101];   /*地面がどこにあるか座標を入れる変数*/

float nowx = 0.0; /*現在位置のｘ座標*/
float nowy = 0.0; /*現在位置のy座標*/
float nowz;

BOOL jump = FALSE; /*進めるかどうか*/

void defaultValue(void);
int changeToBrock(float);
BOOL judge(char);
void Ground(void);
void brock(float, float, float, float, float, float);
void tree(float, float, float);
void rockMountain(float, float, float);
void mob(void);
void display(void);
void idle(void);
void mykbd(unsigned char, int, int);
void myupkbd(unsigned char, int, int);
void myInit(char *);
void myReshape(int, int);
void viewPointkdDown(int, int, int);
void viewPointkdUp(int, int, int);
void setZ(float);

//---------------------------------------------------
// nowzをもとにlookatzとlookfromzを変更
//---------------------------------------------------
void setZ(float ground)
{
  nowz = ground;
  float bufferZ = lookfromz;
  lookfromz = nowz + 1.75;
  bufferZ = lookfromz - bufferZ;
  lookatz += bufferZ;
}

//---------------------------------------------------
// 地形の情報(z軸高さ)を読み込む
//---------------------------------------------------
void defaultValue(void)
{
  FILE *fp;
  int x = changeToBrock(nowx);
  int y = changeToBrock(nowy);
  printf("(%d, %d)\n", x, y);
  fp = fopen("ground.txt", "r");
  if (fp == NULL)
    exit(1);
  for (int i = 0; i < 101; i++)
  {
    for (int j = 0; j < 101; j++)
    {
      ground[i][j] = fgetc(fp) - 48;
      if (ground[i][j] == EOF)
        break;
      if (ground[i][j] == -38)
      {
        /*改行の時*/
        ground[i][j] = fgetc(fp) - 48;
        if (ground[i][j] == EOF)
          break;
      }
    }
  }
  fclose(fp);

  fp = fopen("ground_1.txt", "w");
  if (fp == NULL)
    exit(1);
  for (int i = 0; i < 101; i++)
  {
    for (int j = 0; j <= 101; j++)
    {
      if (j == 101)
      {
        fprintf(fp, "\n");
        if (ground[i][j] == EOF)
          break;
      }
      else
      {
        fprintf(fp, "%d", ground[i][j]);
        if (ground[i][j] == EOF)
          break;
      }
    }
  }
  fclose(fp);
}

//----------------------------------------------------
// 現在地のブロック番号
// 座標をgroundのインデックスに変換
// 引数:float
// 戻り値:int
//----------------------------------------------------
int changeToBrock(float now)
{
  float zero = 0.0;
  int new;
  float a, b; /*aは商、bはあまり*/
  if (now > 0)
  {
    now -= 0.5;
    if (now < 0)
    {
      /*元々のnowが0.5より小さいのとき */
      new = 0;
    }
    else
    {
      a = (int)now;
      b = now - a;
      if (b > 0.0 || 0.0 > b)
      {
        /*あまりがあった時*/
        new = a + 1;
      }
      else
      {
        new = a;
        printf("new;%d\n", new);
      }
    }
  }
  else if (now < 0)
  {
    now += 0.5;
    if (now > 0)
    {
      /*元々のnowが-0.5より大きいとき*/
      new = 0;
    }
    else
    {
      a = (int)now;
      b = now - a;
      if (b > 0.0 || 0.0 > b)
      {
        /*あまりがあった時*/
        new = a - 1;
      }
      else
      {
        new = a;
      }
    }
  }
  else
  {
    new = 0;
  }
  new += map_max_x;
  return new;
}

//----------------------------------------------------
// 進めるかの判定
// 引数:char
// 戻り値:bool
//----------------------------------------------------
BOOL judge(char direction)
{
  int viewdirection;
  BOOL check; /*TRUEのとき進行できる|FALSEのとき進行方向が現在位置より高いため進行できない*/
  int x, y, z;
  float xbuffer = nowx;
  float ybuffer = nowy;
  switch (direction)
  {
  case 'w':
    xbuffer += 1.0 * cos(thetaXY * M_PI / 180);
    ybuffer += 1.0 * sin(thetaXY * M_PI / 180);
    break;
  case 's':
    xbuffer += 1.0 * cos((thetaXY + 180) * M_PI / 180);
    ybuffer += 1.0 * sin((thetaXY + 180) * M_PI / 180);
    break;
  case 'a':
    xbuffer += 1.0 * cos((thetaXY + 90) * M_PI / 180);
    ybuffer += 1.0 * sin((thetaXY - 90) * M_PI / 180);
    break;
  case 'd':
    xbuffer += 1.0 * cos((thetaXY - 90) * M_PI / 180);
    ybuffer += 1.0 * sin((thetaXY - 90) * M_PI / 180);
    break;
  default:
    break;
  }
  printf("judge\n");
  x = changeToBrock(xbuffer);
  y = changeToBrock(ybuffer);
  z = (int)nowz;

  for (int i = x - 12; i <= x + 10; i++)
  {
    for (int j = y - 12; j <= y + 10; j++)
    {
      printf("[%d]", ground[i][j]);
    }
    printf("\n");
  }

  if (ground[x][y] < (int)z)
  { /*進行先の地面が現在位置より下に地面がある時＝着地する（nowzを下げる）*/
    nowz = ground[x][y];
    setZ(nowz);
    check = TRUE;
  }
  else if (ground[x][y] == (int)z)
  { /*進行先の地面が現在位置と同じ時＝進行する*/
    nowz = ground[x][y];
    setZ(nowz);
    check = TRUE;
  }
  else
  {
    check = FALSE;
  }
  printf("key:%c, もし進めたらground[%d][%d]\n", direction, x, y);
  return check;
}

//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void Ground(void)
{
  glPushMatrix();
  glBegin(GL_QUADS);
  glColor3f(0.5, 1.0, 0.5);
  glVertex3f(nowx - ground_max_x - 10.0, nowy + ground_max_y + 10.0, 0.); // 左上頂点
  glVertex3f(nowx + ground_max_x + 10.0, nowy + ground_max_y + 10.0, 0.); // 右上頂点
  glVertex3f(nowx + ground_max_x + 10.0, nowy - ground_max_y - 10.0, 0.); // 右下頂点
  glVertex3f(nowx - ground_max_x - 10.0, nowy - ground_max_y - 10.0, 0.); // 左下頂点
  glEnd();
  glPopMatrix();
}

//----------------------------------------------------
// ブロック作成
// 引数:float, float
//----------------------------------------------------
void brock(float r, float g, float b, float x, float y, float z)
{
  glPushMatrix();
  glTranslatef(x, y, z);
  glColor3f(0.0, 0.0, 0.0);
  glutWireCube(1.00);
  glColor3f(r, g, b);
  glEnable(GL_NORMALIZE);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5);
  glTexCoord2f(0.3, 0.0);
  glVertex3f(-0.5, 0.5, -0.5);
  glTexCoord2f(0.3, 0.5);
  glVertex3f(0.5, 0.5, -0.5);
  glTexCoord2f(0.0, 0.5);
  glVertex3f(0.5, -0.5, -0.5);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.7, 0.5);
  glVertex3f(-0.5, -0.5, 0.5);
  glTexCoord2f(1.0, 0.5);
  glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5);
  glTexCoord2f(0.7, 1.0);
  glVertex3f(0.5, -0.5, 0.5);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.4, 0.0);
  glVertex3f(-0.5, -0.5, -0.5);
  glTexCoord2f(0.7, 0.0);
  glVertex3f(-0.5, -0.5, 0.5);
  glTexCoord2f(0.7, 0.5);
  glVertex3f(0.5, -0.5, 0.5);
  glTexCoord2f(0.4, 0.5);
  glVertex3f(0.5, -0.5, -0.5);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.4, 0.5);
  glVertex3f(-0.5, 0.5, -0.5);
  glTexCoord2f(0.7, 0.5);
  glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(0.7, 1.0);
  glVertex3f(0.5, 0.5, 0.5);
  glTexCoord2f(0.4, 1.0);
  glVertex3f(0.5, 0.5, -0.5);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.7, 0.0);
  glVertex3f(0.5, -0.5, -0.5);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5, 0.5, -0.5);
  glTexCoord2f(1.0, 0.5);
  glVertex3f(0.5, 0.5, 0.5);
  glTexCoord2f(0.7, 0.5);
  glVertex3f(0.5, -0.5, 0.5);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.5);
  glVertex3f(-0.5, -0.5, -0.5);
  glTexCoord2f(0.35, 0.5);
  glVertex3f(-0.5, 0.5, -0.5);
  glTexCoord2f(0.35, 1.0);
  glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5, -0.5, 0.5);
  glEnd();

  glDisable(GL_NORMALIZE);
  glPopMatrix();
}

//----------------------------------------------------
// 木を生成
//----------------------------------------------------
void tree(float x, float y, float z)
{
  glPushMatrix();
  int bx, by;
  /*幹*/
  for (float i = 0.; i < 3.; i += 1.0)
  {
    brock(0.5, 0.2, 0.0, x, y, z + i);
  }
  /*葉*/
  brock(0.0, 0.5, 0.0, x - 1.0, y, z + 3.0);
  brock(0.0, 0.5, 0.0, x + 1.0, y, z + 3.0);
  brock(0.0, 0.5, 0.0, x, y - 1.0, z + 3.0);
  brock(0.0, 0.5, 0.0, x, y + 1.0, z + 3.0);
  brock(0.0, 0.5, 0.0, x, y, z + 4.0);

  bx = changeToBrock(x);
  by = changeToBrock(y);
  ground[bx][by] = 4.0;
  glPopMatrix();
}

//----------------------------------------------------
// 岩山を生成
//----------------------------------------------------
void rockMountain(float theta, float x, float y)
{
  glPushMatrix();
  glRotated(theta, 0.0, 0.0, 1.0);
  glTranslatef(x, y, 0.0);

  brock(0.5, 0.5, 0.5, -3.0, 10.0, 0.5);
  brock(0.5, 0.5, 0.5, -3.0, 9.0, 0.5);
  brock(0.5, 0.5, 0.5, -3.0, 8.0, 0.5);
  brock(0.5, 0.5, 0.5, -4.0, 8.0, 0.5);
  brock(0.5, 0.5, 0.5, -5.0, 8.0, 0.5);
  brock(0.5, 0.5, 0.5, -6.0, 8.0, 0.5);
  brock(0.5, 0.5, 0.5, -6.0, 7.0, 0.5);
  brock(0.5, 0.5, 0.5, -6.0, 6.0, 0.5);
  brock(0.5, 0.5, 0.5, -7.0, 6.0, 0.5);
  brock(0.5, 0.5, 0.5, -8.0, 6.0, 0.5);
  brock(0.5, 0.5, 0.5, -8.0, 5.0, 0.5);
  brock(0.5, 0.5, 0.5, -8.0, 4.0, 0.5);
  brock(0.5, 0.5, 0.5, -8.0, 3.0, 0.5);
  brock(0.5, 0.5, 0.5, -9.0, 3.0, 0.5);
  brock(0.5, 0.5, 0.5, -10.0, 3.0, 0.5);

  brock(0.5, 0.5, 0.5, -3.0, 3.0, 0.5);

  brock(0.5, 0.5, 0.5, -4.0, 10.0, 1.5);
  brock(0.5, 0.5, 0.5, -4.0, 9.0, 1.5);
  brock(0.5, 0.5, 0.5, -5.0, 9.0, 1.5);
  brock(0.5, 0.5, 0.5, -6.0, 9.0, 1.5);
  brock(0.5, 0.5, 0.5, -6.0, 8.0, 1.5);
  brock(0.5, 0.5, 0.5, -7.0, 7.0, 1.5);
  brock(0.5, 0.5, 0.5, -8.0, 7.0, 1.5);
  brock(0.5, 0.5, 0.5, -8.0, 6.0, 1.5);
  brock(0.5, 0.5, 0.5, -9.0, 6.0, 1.5);
  brock(0.5, 0.5, 0.5, -9.0, 5.0, 1.5);
  brock(0.5, 0.5, 0.5, -9.0, 4.0, 1.5);
  brock(0.5, 0.5, 0.5, -10.0, 4.0, 1.5);

  brock(0.5, 0.5, 0.5, -5.0, 10.0, 2.5);
  brock(0.5, 0.5, 0.5, -6.0, 10.0, 2.5);
  brock(0.5, 0.5, 0.5, -6.0, 9.0, 2.5);
  brock(0.5, 0.5, 0.5, -7.0, 10.0, 2.5);
  brock(0.5, 0.5, 0.5, -7.0, 9.0, 2.5);
  brock(0.5, 0.5, 0.5, -7.0, 8.0, 2.5);
  brock(0.5, 0.5, 0.5, -8.0, 8.0, 2.5);
  brock(0.5, 0.5, 0.5, -8.0, 7.0, 2.5);
  brock(0.5, 0.5, 0.5, -9.0, 7.0, 2.5);
  brock(0.5, 0.5, 0.5, -9.0, 6.0, 2.5);
  brock(0.5, 0.5, 0.5, -10.0, 6.0, 2.5);
  brock(0.5, 0.5, 0.5, -10.0, 5.0, 2.5);

  brock(0.5, 0.5, 0.5, -8.0, 10.0, 3.5);
  brock(0.5, 0.5, 0.5, -8.0, 9.0, 3.5);
  brock(0.5, 0.5, 0.5, -9.0, 10.0, 3.5);
  brock(0.5, 0.5, 0.5, -9.0, 9.0, 3.5);
  brock(0.5, 0.5, 0.5, -9.0, 8.0, 3.5);
  brock(0.5, 0.5, 0.5, -9.0, 7.0, 3.5);
  brock(0.5, 0.5, 0.5, -10.0, 10.0, 3.5);
  brock(0.5, 0.5, 0.5, -10.0, 9.0, 3.5);
  brock(0.5, 0.5, 0.5, -10.0, 8.0, 3.5);
  brock(0.5, 0.5, 0.5, -10.0, 7.0, 3.5);
  brock(0.5, 0.5, 0.5, -10.0, 6.0, 3.5);
  glPopMatrix();
}

//---------------------------------------------------
// mobを生成
//---------------------------------------------------
void mob(void)
{
  glEnable(GL_NORMALIZE);
  /*頭*/
  glPushMatrix();
  glTranslatef(0.0, 0.0, 1.5);
  glColor3f(1.0, 1.0, 1.0);
  glutSolidCube(1.0);
  glPopMatrix();

  /*胴*/
  glPushMatrix();
  glTranslatef(0.5, 0.0, 0.5);
  glColor3f(1.0, 1.0, 1.0);
  glScalef(2.0, 1.0, 1.0);
  glutSolidCube(1.0);
  glPopMatrix();

  /*とさか*/
  glPushMatrix();
  glTranslatef(0.25, 0.0, 2.25);
  glColor3f(1.0, 0.0, 0.0);
  glScalef(2.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);
  glutSolidCube(1.0);
  glPopMatrix();

  /*くちばし*/
  glPushMatrix();
  glTranslatef(-0.75, 0.0, 1.5);
  glColor3f(1.0, 1.0, 0.0);
  glScalef(1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);
  glutSolidCube(1.0);
  glPopMatrix();

  /*しっぽ*/
  glPushMatrix();
  glTranslatef(1.75, 0.0, 0.5);
  glColor3f(1.0, 1.0, 1.0);
  glScalef(1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);
  glutSolidCube(1.0);
  glPopMatrix();

  glDisable(GL_NORMALIZE);
}

void display(void)
{
  glPushMatrix(); /*視点座標系*/
  gluLookAt(
      lookfromx, lookfromy, lookfromz,                // 視点の位置x,y,z;
      lookatx, lookaty, lookatz,                      // 視界の中心位置の参照点座標x,y,z
      0.0, 0.0, 1.0);                                 // 視界の上方向のベクトルx,y,z
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // バッファの消去
  glEnable(GL_DEPTH_TEST);
  Ground();
  rockMountain(0.0, 0.0, 0.0);
  rockMountain(270.0, 0.0, -21.0);
  rockMountain(180.0, 21.0, -21.0);
  rockMountain(90.0, 21.0, 0.0);

  tree(5.0, 5.0, 0.0);
  tree(-8.0, -5.0, 0.0);

  glPushMatrix();
  glEnable(GL_NORMALIZE);
  glTranslated(mobx, moby, mobz);
  if (flag == 0)
  {
    glRotated(180, 0, 0, 1.0);
  }
  glPushMatrix();
  glScalef(0.15, 0.15, 0.15);
  mob();
  glPopMatrix();
  glDisable(GL_NORMALIZE);
  glPopMatrix();
  glutSwapBuffers(); // glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
  glPopMatrix();     /*視点座標系*/
  glDisable(GL_DEPTH_TEST);
}

void idle(void)
{
  int x = changeToBrock(nowx);
  int y = changeToBrock(nowy);
  if (jump == FALSE)
  {
    setZ(ground[x][y]);
  }

  if (flag == 0)
  {
    mobx += 0.001;
    if (mobx >= 50)
    {
      flag = 1;
    }
  }
  else
  {
    mobx -= 0.001;
    if (mobx <= -50)
    {
      flag = 0;
    }
  }
  glutPostRedisplay();
}

//------------------------------------------------------
// キーボード入力（入力された瞬間）
//------------------------------------------------------

void mykbd(unsigned char key, int x, int y)
{
  x = changeToBrock(nowx);
  y = changeToBrock(nowy);

  if (judge(key) == FALSE)
    return;

  switch (key)
  {
  case 'w':
    nowx += 1.0 * cos(thetaXY * M_PI / 180);
    lookfromx += 1.0 * cos(thetaXY * M_PI / 180);
    lookatx += 1.0 * cos(thetaXY * M_PI / 180);
    nowy += 1.0 * sin(thetaXY * M_PI / 180);
    lookfromy += 1.0 * sin(thetaXY * M_PI / 180);
    lookaty += 1.0 * sin(thetaXY * M_PI / 180);
    break;
  case 's':
    nowx += 1.0 * cos((thetaXY + 180) * M_PI / 180);
    lookfromx += 1.0 * cos((thetaXY + 180) * M_PI / 180);
    lookatx += 1.0 * cos((thetaXY + 180) * M_PI / 180);
    nowy += 1.0 * sin((thetaXY + 180) * M_PI / 180);
    lookfromy += 1.0 * sin((thetaXY + 180) * M_PI / 180);
    lookaty += 1.0 * sin((thetaXY + 180) * M_PI / 180);
    break;
  case 'a':
    nowx += 1.0 * cos((thetaXY + 90) * M_PI / 180);
    lookfromx += 1.0 * cos((thetaXY + 90) * M_PI / 180);
    lookatx += 1.0 * cos((thetaXY + 90) * M_PI / 180);
    nowy += 1.0 * sin((thetaXY - 90) * M_PI / 180);
    lookfromy += 1.0 * sin((thetaXY + 90) * M_PI / 180);
    lookaty += 1.0 * sin((thetaXY + 90) * M_PI / 180);
    break;
  case 'd':
    nowx += 1.0 * cos((thetaXY - 90) * M_PI / 180);
    lookfromx += 1.0 * cos((thetaXY - 90) * M_PI / 180);
    lookatx += 1.0 * cos((thetaXY - 90) * M_PI / 180);
    nowy += 1.0 * sin((thetaXY - 90) * M_PI / 180);
    lookfromy += 1.0 * sin((thetaXY - 90) * M_PI / 180);
    lookaty += 1.0 * sin((thetaXY - 90) * M_PI / 180);
    break;
  case ' ':
    nowz = ground[x][y] + 1;
    setZ(nowz);
    jump = TRUE;
    break;
  case KEY_ESC:
    exit(0);
    break;
  default:
    break;
  }
  printf("mykd\n");
  printf("lookat(%f, %f, %f) lookfrom(%f, %f, %f) thetaXY:%f\n", lookatx, lookaty, lookatz, lookfromx, lookfromy, lookfromz, thetaXY);
  defaultValue();
  glutPostRedisplay();
}

//-----------------------------------------------------------
// キーボード入力（離れた瞬間）
//-----------------------------------------------------------
void myupkbd(unsigned char key, int x, int y)
{
  x = changeToBrock(nowx);
  y = changeToBrock(nowy);
  switch (key)
  {
  case ' ':
    jump = FALSE;
    nowz = ground[x][y];
    setZ(nowz);
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

//----------------------------------------------------------
// 視点移動（入力された瞬間）
//----------------------------------------------------------
void viewPointkdDown(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    lookatz = lookfromz + 0.1;
    break;
  case GLUT_KEY_DOWN:
    lookatz = lookfromz - 0.3;
    break;
  case GLUT_KEY_LEFT:
    thetaXY += 15;
    break;
  case GLUT_KEY_RIGHT:
    thetaXY -= 15;
    break;
  default:
    break;
  }

  lookatx = lookfromx + 0.5 * cos(thetaXY * M_PI / 180);
  lookaty = lookfromy + 0.5 * sin(thetaXY * M_PI / 180);
  printf("lookat(%f, %f, %f) lookfrom(%f, %f, %f) thetaXY:%f\n", lookatx, lookaty, lookatz, lookfromx, lookfromy, lookfromz, thetaXY);
  glutPostRedisplay();
}

//------------------------------------------------------------
// 視点移動（離れた瞬間）
//------------------------------------------------------------
void viewPointkdUp(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    lookatz = lookfromz - 0.15;
    break;
  case GLUT_KEY_DOWN:
    lookatz = lookfromz - 0.15;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void myInit(char *progname)
{
  float aspect = 2160.0 / 1080.0;
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(2160, 1080);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH); /*GLUT_DEPTHでZバッファの利用を宣言*/
  glutCreateWindow(progname);
  glClearColor(0.0, 0.8, 1.0, 1.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, aspect, 0.1, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

void myReshape(int width, int height)
{
  float aspect = (float)width / (float)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, aspect, 0.1, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
  printf("lookat(%f, %f, %f)\n", lookatx, lookaty, lookatz);
  glutInit(&argc, argv);
  myInit(argv[0]);
  glutReshapeFunc(myReshape); /*ウィンドウのサイズが変化した場合に再描画を行う*/
  defaultValue();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(mykbd);
  glutKeyboardUpFunc(myupkbd);
  glutSpecialFunc(viewPointkdDown);
  glutSpecialUpFunc(viewPointkdUp);
  glutMainLoop();
  return (0);
}