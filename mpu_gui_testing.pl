#!/usr/bin/perl

use Tk;
use threads;
use threads::shared;
use Device::SerialPort;
use Time::HiRes qw(usleep nanosleep);
use OpenGL::Modern;
  BEGIN{ unshift(@INC,"../blib");} # in case OpenGL is built but not installed
use OpenGL;

my $ch : shared = 'a';
my $sh : shared = 'b';
my $angle_X : shared = '';
my $angle_Y : shared = '';
my $angle_Z : shared = '';
my $x : shared = 0;
my $y : shared = 0;
my $z : shared = 0;
my $r : shared = 0;
my $cnt : shared = 1;

my $sp = Device::SerialPort->new("/dev/ttyUSB1");
die "Can't open /dev/ttyUSB1: $!" if ! defined $sp;
$sp->baudrate(9600);
$sp->databits(8);
$sp->parity("none");
$sp->stopbits(1);




$th1=threads->new(\&thr1); # Serial Read
$th2=threads->new(\&thr2); # GUI-Tk

$th1->join();
$th2->join();





#************
# THREAD #1 *
#************
sub thr1
{
  while(1){
    $ch = $sp->read(1);
    if($ch eq '$' and $msg==1){ print "x: $angle_X : ";
                                $x = $angle_X+0; print "$x\n";
                                $angle_X =''; $msg = 0;}
    if($ch eq '$' and $msg==2){ print "y: $angle_Y : ";
                                $y = $angle_Y+0; print "$y\n";
                                $angle_Y =''; $msg = 0;}
    if($ch eq '$' and $msg==3){ print "z: $angle_Z : ";
                                $z = $angle_Z+0; print "$z\n";
                                $angle_Z =''; $msg = 0;}



    if($msg == 1){ $angle_X = $angle_X.$ch;}
    if($msg == 2){ $angle_Y = $angle_Y.$ch;}
    if($msg == 3){ $angle_Z = $angle_Z.$ch;}

    if($ch eq 'X'){ $msg = 1; }
    if($ch eq 'Y'){ $msg = 2; }
    if($ch eq 'Z'){ $msg = 3; }



  }
}


#************
# THREAD #2 *
#************
sub thr2
{
  $mw = MainWindow -> new(-bg => black);

  $mw -> geometry("1020x680");
  $mw-> waitVisibility;

  $cv=$mw->Canvas(-width=>1025, -height=>300, -borderwidth=>0, -bd=>0,
                                -highlightthickness=>0, -relief=>'ridge',
                                -bg => 'black')->place(-x=>0, -y=>0);

  #$ln = $cv -> createLine(20,20,80,80, -fill => red, width => 2);
  $arc1 = $cv -> createArc(20,20,200,200, -outline => blue, -extent => 180,
                                        -start => 0);
  #$rec = $cv -> createRectangle(20,20,200,200, -outline => red);
  $arc2 = $cv -> createArc(320,20,500,200, -outline => blue, -extent => 180,
                                          -start => 0);

  $arc3 = $cv -> createArc(620,20,800,200, -outline => blue, -extent => 180,
                                         -start => $z);



  $ogl = glpOpenWindow(parent=> hex($mw->id), width => 450, height => 450,
   x =>250, y => 220);


  glShadeModel(GL_FLAT);
  myReshape();
  display();

  #---------------
  #- TEXT LABELS -
  #---------------
  $mw -> Label (-text => "X  = ", -fg => green, -bg => black)
      ->place (-x => 75, -y => 150);
  $mw -> Label (-text => "Y  = ", -fg => green, -bg => black)
      ->place (-x => 375, -y => 150);
  $mw -> Label (-text => "Z  = ", -fg => green, -bg => black)
      ->place (-x => 675, -y => 150);
  $mw -> Label (-text => "[dgr]", -fg => green, -bg => black)
      ->place (-x => 145, -y => 150);
  $mw -> Label (-text => "[dgr]", -fg => green, -bg => black)
      ->place (-x => 445, -y => 150);
  $mw -> Label (-text => "[dgr]", -fg => green, -bg => black)
      ->place (-x => 745, -y => 150);
  $lx = $mw -> Label(-text => $x, -fg => green, -bg => black);
  $lx->place(-x => 105, -y => 150);
  $ly = $mw -> Label(-text => $y, -fg => green, -bg => black);
  $ly->place(-x => 405, -y => 150);
  $lz = $mw -> Label(-text => $z, -fg => green, -bg => black);
  $lz->place(-x => 705, -y => 150);


  $mw-> repeat(1, \&move);

  MainLoop();


  sub move {
    $cv -> itemconfigure($arc1, -start => $x);
    $cv -> itemconfigure($arc2, -start => $y);
    $cv -> itemconfigure($arc3, -start => $z);
    $lx ->configure (-text => $x);
    $ly ->configure (-text => $y);
    $lz ->configure (-text => $z);
    #$mw->update();

    if($r<720){
    $r++;}
    display();
  }


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
      glRotatef($x, 1.0, 0.0, 0.0);
      glRotatef($y, 0.0, 0.0, 1.0);
      glRotatef($z, 0.0, 1.0, 0.0);
      glTranslatef(0.0, 0.0, 0.0);


      	#  viewing transformation

      glScalef(1.5, 0.6, 1.5);	#  modeling transformation
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



}#thr2
