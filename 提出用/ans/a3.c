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
	glColor3f( 1.0, 1.0, 0.0 );/*�A�`�敨�̂ɔ��F��ݒ�=�����̒l��0.0~1.0*/
	glBegin( GL_POLYGON ); /*�B�|���S���i���p�`���ʁj�̕`��J�n*/
		glVertex3f( 0.1, 0.1, 0.0 );/*�C�e���_���W�̐ݒ�*/
		glVertex3f(-0.1, 0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		glVertex3f(-0.1,-0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		glVertex3f( 0.1,-0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
	glEnd();/*�D�|���S���̕`��I��*/



	glLoadIdentity();
	glColor3f( 0.0, 0.0, 0.0 );/*�A�`�敨�̂ɔ��F��ݒ�=�����̒l��0.0~1.0*/
	glBegin( GL_LINE_LOOP ); /*�B�|���S���i���p�`���ʁj�̕`��J�n*/
		glVertex3f( 0.1, 0.1, 0.0 );/*�C�e���_���W�̐ݒ�*/
		glVertex3f(-0.1, 0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		glVertex3f(-0.1,-0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		glVertex3f( 0.1,-0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
	glEnd();/*�D�|���S���̕`��I��*/
	my_axis();

	glTranslatef(0.25,0.,0.);
	glRotatef(90.,0.,0.,1.);
	glScalef(1.,1.5,1.);
	glColor3f( 0.0, 1.0, 0.0 );/*�A�`�敨�̂ɗΐF��ݒ�*/
	glBegin( GL_POLYGON ); /*�B�|���S���i���p�`���ʁj�̕`��J�n*/
		glVertex3f( 0.1, 0.1, 0.0 );/*�C�e���_���W�̐ݒ�*/
		glVertex3f(-0.1, 0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		glVertex3f(-0.1,-0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		glVertex3f( 0.1,-0.1, 0.0 ); /*�C�e���_���W�̐ݒ�*/
	glEnd();/*�D�|���S���̕`��I��*/

	glFlush();
}

void my_axis()
{
	int i;
	glLoadIdentity();
	glBegin( GL_LINES ); 
	 	glColor3f( 1.0, 0.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );/*�C�e���_���W�̐ݒ�*/
		glVertex3f(-1.0, 0.0, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		for(i=0;i<20;i++){
			glVertex3f(-1+0.1*i, -0.05, 0.0 );/*�C�e���_���W�̐ݒ�*/
			glVertex3f(-1+0.1*i,  0.05, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		}	
	 	glColor3f( 0.0, 0.0, 1.0 );
		glVertex3f( 0.,-1.0, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		glVertex3f( 0., 1.0, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		for(i=0;i<20;i++){
			glVertex3f(-0.05, -1+0.1*i, 0.0 );/*�C�e���_���W�̐ݒ�*/
			glVertex3f( 0.05, -1+0.1*i, 0.0 ); /*�C�e���_���W�̐ݒ�*/
		}	
	glEnd();/*�D�|���S���̕`��I��*/	
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

	glMatrixMode( GL_PROJECTION );/*�@���W�n�ƕ`��̈�̒�`*/
	glLoadIdentity();/* �@���W�n��ݒ�i���s���e�j*/
	glOrtho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );/* �@���W�n��ݒ�i���s���e�j*/
}


int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	myInit( argv[0] );
	glutDisplayFunc( display );
	glutMainLoop();
	return( 0 );
}
