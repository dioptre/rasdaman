#!/usr/local/dist/bin/perl -w
# use this one on Linux
#!/usr/bin/perl -w

# This program generates query files for use with test_benchmark. It
# generates all files needed for executing the measurements specified 
# in the PhD thesis. It is not too generic, most of the things are
# hardcoded.

# each query is repeated 20 times
$repeat = 20;
# the collections which are used.
@mdd = ( tomo_cubed, tomo_cubed_16, tomo_cubed_64, tomo_sliced );
# the file name suffixes used for the collections
@suffixes = ( _32, _16, _64, _s );
# the selectivities tested for each collection
@selectivity = ( .5, 1, 2, 5, 10, 20, 50 );
# the filenames used to store the results

sub calcQueryBox($$$$$$$$$);
sub randNum($$);
# initialize rand()
srand();

sub randNum($$)
{
    my ($l, $h) = @_;

    return int ( $l + rand()*($h-$l) );
}

# this function calculates a random query box given the spatial
# 
sub calcQueryBox($$$$$$$$$)
{
    my ( $xl, $xh, $yl, $yh, $zl, $zh, $xd, $yd, $zd ) = @_;

    $x1 = randNum( $xl, $xh - $xd + 1);
    $x2 = $x1 + ( $xd - 1 > 0 ? $xd - 1 : 0 );
    $y1 = randNum( $yl, $yh - $yd + 1);
    $y2 = $y1 + ( $yd - 1 > 0 ? $yd - 1 : 0 );
    $z1 = randNum( $zl, $zh - $zd + 1);
    $z2 = $z1 + ( $zd - 1 > 0 ? $zd - 1 : 0 );

    return "[$x1:$x2, $y1:$y2, $z1:$z2]";
}

$headerStr = "
// Testing acces to tomo_cubed with moving query box on different
// selectivities.  
";

foreach $tomo (@mdd)
{
    $suffix = shift @suffixes;
    $fName1 = "tcubemov" . $suffix . ".ql";
    $fName2 = "tcubeavg" . $suffix . ".ql";
    open(QUERY1,">$fName1");
    open(QUERY2,">$fName2");
    print QUERY1 "$headerStr\n\n";
    print QUERY2 "$headerStr\n\n";
    foreach $sel (@selectivity)
    {
	$dx = $dy = int ( ($sel/100)**(1/3) * 256 ); 
	$dz = int ( ($sel/100)**(1/3) * 154 );
	# adapt query box for better fit
	if(++$dx * $dy * $dz <= $sel/100*256*256*154) {
	    if($dx * ++$dy * $dz <= $sel/100*256*256*154) {
		if($dx * $dy * ++$dz <= $sel/100*256*256*154) {
		}
		else {
		    $dz--;
		}
	    }
	    else {
		$dy--;
	    }
	}
	else {
	    $dx--;
	}
	for ($i = 1; $i <= $repeat; $i++) {
	    $qBox = calcQueryBox( 0, 255, 0, 255, 0, 153, $dx, $dy, $dz );
	    # print Query
	    print QUERY1 "// [$sel]: $qBox\n";
	    print QUERY2 "// [$sel]: $qBox\n";
	    print QUERY1 "SELECT img$qBox\nFROM $tomo AS img\n";
	    print QUERY2 "SELECT avg_cells(img$qBox)\nFROM $tomo AS img\n";
	}
    }
}

@suffixes = ( _32, _16, _64, _s );

foreach $tomo (@mdd)
{
    $suffix = shift @suffixes;
    $fName1 = "tslicemov" . $suffix . ".ql";
    $fName2 = "tsliceavg" . $suffix . ".ql";
    open(QUERY1,">$fName1");
    open(QUERY2,">$fName2");
    print QUERY1 "$headerStr\n\n";
    print QUERY2 "$headerStr\n\n";
    foreach $sel (@selectivity)
    {
	$dx = int ( ($sel/100) * 256 );
	for ($i = 1; $i <= $repeat; $i++) {
	    $qBox = calcQueryBox( 0, 255, 0, 255, 0, 153, $dx, 256, 154 );
	    # print Query
	    print QUERY1 "// [$sel]: $qBox\n";
	    print QUERY2 "// [$sel]: $qBox\n";
	    print QUERY1 "SELECT img$qBox\nFROM $tomo AS img\n";
	    print QUERY2 "SELECT avg_cells(img$qBox)\nFROM $tomo AS img\n";
	}
    }
}
