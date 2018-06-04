#!/usr/bin/perl

use Tk;
use threads;
use threads::shared;
use Device::SerialPort;
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
  $mw = MainWindow -> new();

  $mw -> geometry("1020x680");

  $cv=$mw->Canvas(-width=>1000, -height=>670,
                                -background => 'black')->place(-x=>5, -y=>5);

  #$ln = $cv -> createLine(20,20,80,80, -fill => red, width => 2);
  $arc1 = $cv -> createArc(20,20,200,200, -outline => blue, -extent => 180,
                                        -start => 0);
  #$rec = $cv -> createRectangle(20,20,200,200, -outline => red);
  $arc2 = $cv -> createArc(320,20,500,200, -outline => blue, -extent => 180,
                                          -start => 0);

  $arc3 = $cv -> createArc(620,20,800,200, -outline => blue, -extent => 180,
                                         -start => $z);

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


  $mw-> repeat(5, \&move);

  MainLoop();


  sub move {
    $cv -> itemconfigure($arc1, -start => $x);
    $cv -> itemconfigure($arc2, -start => $y);
    $cv -> itemconfigure($arc3, -start => $z);
    $lx ->configure (-text => $x);
    $ly ->configure (-text => $y);
    $lz ->configure (-text => $z);
    #$mw->update();

   }


}
