/*  c1-2.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#define KEY_ESC 27 

void display( void )
{
	void my_axis();

	glClear( GL_COLOR_BUFFER_BIT );
	
	glTranslatef(-0.2,0.,0.);
	glScalef(1.,4.,1.);
	glColor3f( 1.0, 1.0, 0.0 );/*②描画物体に白色を設定=引数の値は0.0~1.0*/
	glBegin( GL_POLYGON ); /*③ポリゴン（多角形平面）の描画開始*/
		glVertex3f( 0.1, 0.1, 0.0 );/*④各頂点座標の設定*/
		glVertex3f(-0.1, 0.1, 0.0 ); /*④各頂点座標の設定*/
		glVertex3f(-0.1,-0.1, 0.0 ); /*④各頂点座標の設定*/
		glVertex3f( 0.1,-0.1, 0.0 ); /*④各頂点座標の設定*/
	glEnd();/*⑤ポリゴンの描画終了*/



	glLoadIdentity();
	glColor3f( 0.0, 0.0, 0.0 );/*②描画物体に白色を設定=引数の値は0.0~1.0*/
	glBegin( GL_LINE_LOOP ); /*③ポリゴン（多角形平面）の描画開始*/
		glVertex3f( 0.1, 0.1, 0.0 );/*④各頂点座標の設定*/
		glVertex3f(-0.1, 0.1, 0.0 ); /*④各頂点座標の設定*/
		glVertex3f(-0.1,-0.1, 0.0 ); /*④各頂点座標の設定*/
		glVertex3f( 0.1,-0.1, 0.0 ); /*④各頂点座標の設定*/
	glEnd();/*⑤ポリゴンの描画終了*/
	my_axis();

	glTranslatef(0.25,0.,0.);
	glRotatef(90.,0.,0.,1.);
	glScalef(1.,1.5,1.);
	glColor3f( 0.0, 1.0, 0.0 );/*②描画物体に緑色を設定*/
	glBegin( GL_POLYGON ); /*③ポリゴン（多角形平面）の描画開始*/
		glVertex3f( 0.1, 0.1, 0.0 );/*④各頂点座標の設定*/
		glVertex3f(-0.1, 0.1, 0.0 ); /*④各頂点座標の設定*/
		glVertex3f(-0.1,-0.1, 0.0 ); /*④各頂点座標の設定*/
		glVertex3f( 0.1,-0.1, 0.0 ); /*④各頂点座標の設定*/
	glEnd();/*⑤ポリゴンの描画終了*/

	glFlush();
}

void my_axis()
{
	int i;
	glLoadIdentity();
	glBegin( GL_LINES ); 
	 	glColor3f( 1.0, 0.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );/*④各頂点座標の設定*/
		glVertex3f(-1.0, 0.0, 0.0 ); /*④各頂点座標の設定*/
		for(i=0;i<20;i++){
			glVertex3f(-1+0.1*i, -0.05, 0.0 );/*④各頂点座標の設定*/
			glVertex3f(-1+0.1*i,  0.05, 0.0 ); /*④各頂点座標の設定*/
		}	
	 	glColor3f( 0.0, 0.0, 1.0 );
		glVertex3f( 0.,-1.0, 0.0 ); /*④各頂点座標の設定*/
		glVertex3f( 0., 1.0, 0.0 ); /*④各頂点座標の設定*/
		for(i=0;i<20;i++){
			glVertex3f(-0.05, -1+0.1*i, 0.0 );/*④各頂点座標の設定*/
			glVertex3f( 0.05, -1+0.1*i, 0.0 ); /*④各頂点座標の設定*/
		}	
	glEnd();/*⑤ポリゴンの描画終了*/	
}


void myKbd( unsigned char key, int x, int y )
{
	if ( key == KEY_ESC ) exit( 0 );
}


void myInit ( char *progname )
{
	int width = 640, height = 480;
	int screen_width,screen_height,x0,y0;

	screen_width = glutGet( GLUT_SCREEN_WIDTH);
	screen_height = glutGet( GLUT_SCREEN_HEIGHT);

	x0 = (screen_width - width)/2.;
	y0 = (screen_height - height)/2.;
	glutInitWindowPosition( x0, y0 ); 
	glutInitWindowSize( width, height );
	glutInitDisplayMode( GLUT_RGBA );
	glutCreateWindow( progname );

	glClearColor ( 1.0, 1.0, 1.0, 1.0 );
	glutKeyboardFunc( myKbd );

	glMatrixMode( GL_PROJECTION );/*①座標系と描画領域の定義*/
	glLoadIdentity();/* ①座標系を設定（平行投影）*/
	glOrtho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );/* ①座標系を設定（平行投影）*/
}


int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	myInit( argv[0] );
	glutDisplayFunc( display );
	glutMainLoop();
	return( 0 );
}
