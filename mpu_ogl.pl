#!/usr/bin/perl

use OpenGL::Modern;
  BEGIN{ unshift(@INC,"../blib");} # in case OpenGL is built but not installed
use OpenGL;
use Time::HiRes qw(usleep nanosleep);


sub wirecube{
    # adapted from libaux
    local($s) = @_;
    local(@x,@y,@z,@f);
    local($i,$j,$k);
    $s = $s/2.0;
    @x=(-$s,-$s,-$s,-$s,$s,$s,$s,$s);
    @y=(-$s,-$s,$s,$s,-$s,-$s,$s,$s);
    @z=(-$s,$s,$s,-$s,-$s,$s,$s,-$s);
    @f=(
	0, 1, 2, 3,
	3, 2, 6, 7,
	7, 6, 5, 4,
	4, 5, 1, 0,
	5, 6, 2, 1,
	7, 4, 0, 3,
	);
    for($i=0;$i<6;$i++){
        glBegin(GL_LINE_LOOP);
	for($j=0;$j<4;$j++){
		$k=$f[$i*4+$j];
		glVertex3d($x[$k],$y[$k],$z[$k]);
	}
        glEnd();
    }
}
sub display{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glLoadIdentity();	#  clear the matrix
    #kp körül rotálás.. ($r:angle, x , y, z)
    glTranslatef(0.0, 0.0, -5.0);
    glRotatef($r, 1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.0, 0.0);	#  viewing transformation
    #kp körül rotálás vége
    #x, y, z hossz arányok változtatása
    glScalef(0.6, 1.5, 1.5);	#  modeling transformation
    wirecube(1.7);
    glpFlush();
}


sub myReshape {
    # glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}


glpOpenWindow;
glShadeModel(GL_FLAT);
myReshape();
#rotálást hívó fv.
while(1){
  $r++;
  usleep(10000);
  display();
}

glpMainLoop;
