#!/usr/local/dist/bin/perl -w
# use this one on Linux
#!/usr/bin/perl -w

# This program is an adaptation of gen_query for Sequoia 2000 
# queries. 

# IMPORTANT: At the moment this works only for collections 
# containing only one image. WHERE clauses on the OID have to
# be added. Question: How do I get the OIDs? Probably have to
# store them in the insertion program.

# each query is repeated 40 times
$repeat = 40;

sub calcQueryBox($$$$$$);
sub randNum($$);
# initialize rand()
srand();

sub randNum($$)
{
    my ($l, $h) = @_;

    return int ( $l + rand()*($h-$l) );
}

# this function calculates a random query box given the spatial
# domain of the MDD and the extent of the query box
sub calcQueryBox($$$$$$)
{
    my ( $xl, $xh, $yl, $yh, $xd, $yd ) = @_;

    $x1 = randNum( $xl, $xh - $xd + 1);
    $x2 = $x1 + ( $xd - 1 > 0 ? $xd - 1 : 0 );
    $y1 = randNum( $yl, $yh - $yd + 1);
    $y2 = $y1 + ( $yd - 1 > 0 ? $yd - 1 : 0 );

    return "[$x1:$x2, $y1:$y2]";
}

$headerStr = "
// Testing acces to s2k with a moving query box and a random band

";

open(QUERY1,">s2k.ql");

print QUERY1 $headerStr;

for ($i = 1; $i <= $repeat; $i++) {
  $qBox = calcQueryBox( 0, 2599, 0, 1599, 200, 200 );
  $band = randNum(1,5);
  # print Query
  print QUERY1 "// [Query 2]: $qBox\n";
  print QUERY1 "SELECT img$qBox.band$band\nFROM s2k AS img\n";
}

for ($i = 1; $i <= $repeat; $i++) {
  $qBox = calcQueryBox( 0, 2599, 0, 1599, 200, 200 );
  $band = randNum(1,5);
  # print Query
  print QUERY1 "// [Query 3]: $qBox\n";
  print QUERY1 "SELECT (1l*img$qBox.band1 + img$qBox.band2 +\n           img$qBox.band3 + img$qBox.band4 +\n           img$qBox.band5) / 5\nFROM s2k AS img\n";
}
