#!/usr/local/dist/bin/perl

$repeat = 20;

sub randNum($$);
# initialize rand()
srand();

sub randNum
{
    my ($l, $h) = @_;

    return int ( $l + rand()*($h-$l) );
}

sub rollZ
{
    my $tomo = @_[0];
    for ($i = 1; $i <= $repeat; $i++) {
	$start = randNum(0, 153-6);
	print "// [\"z\"] $tomo [*:*, *:*, $start:";
	print $start + 6;
	print "]\n";
	print "SELECT img[*:*,*:*,$start]";
	for ($j = $start + 1; $j <= $start + 6; $j++) {
	    print "+img[*:*,*:*,$j]";
	}
	print "\nFROM $tomo as img\n";
    }
}

sub rollY
{
    my $tomo = @_[0];
    for ($i = 1; $i <= $repeat; $i++) {
	$start = randNum(0, 255-6);
	print "// [\"y\"] $tomo [*:*, $start:";
	print $start + 6;
	print ", *:*]\n";
	print "SELECT img[*:*,$start,*:*]";
	for ($j = $start + 1; $j <= $start + 6; $j++) {
	    print "+img[*:*,$j,*:*]";
	}
	print "\nFROM $tomo as img\n";
    }
}

sub rollX
{
    my $tomo = @_[0];
    for ($i = 1; $i <= $repeat; $i++) {
	$start = randNum(0, 255-6);
	print "// [\"x\"] $tomo [$start:";
	print $start + 6;
	print ",*:*,*:*]\n";
	print "SELECT img[$start,*:*,*:*]";
	for ($j = $start + 1; $j <= $start + 6; $j++) {
	    print "+img[$j,*:*,*:*]";
	}
	print "\nFROM $tomo as img\n";
    }
}

sub SQLrollX
{
    my $tomo = shift;

    for($i = 1; $i <= $repeat; $i++)
    {
	$start = randNum(0, 153-6);
	$end = $start + 6;
	print "
--#COMMENT BETWEEN $start AND $end

SELECT   y, z, sum(val)
FROM     $tomo
WHERE    x BETWEEN $start AND $end
GROUP BY y, z
;
            ";
	
    
    }
}

sub SQLrollY
{
    my $tomo = shift;

    for($i = 1; $i <= $repeat; $i++)
    {
	$start = randNum(0, 255-6);
	$end = $start + 6;
	print "
--#COMMENT BETWEEN $start AND $end

SELECT   x, z, sum(val)
FROM     $tomo
WHERE    y BETWEEN $start AND $end
GROUP BY x, z
;
            ";
	
    
    }
}

sub SQLrollZ
{
    my $tomo = shift;

    for($i = 1; $i <= $repeat; $i++)
    {
	$start = randNum(0, 255-6);
	$end = $start + 6;
	print "
--#COMMENT BETWEEN $start AND $end

SELECT   x, y, sum(val)
FROM     $tomo
WHERE    z BETWEEN $start AND $end
GROUP BY x, y
;
            ";
	
    
    }
}

sub genRollXYZ
{
    $headerStr = "
// Testing roll up operations on different axes. It adds seven
// consecutive slices along the corresponding Axis with a random 
// starting point on all four tomos..

";
 
    @mdd = ( tomo_sliced, tomo_cubed_16, tomo_cubed, tomo_cubed_64 );
    foreach $tomo (@mdd)
    {
	rollX($tomo);
	rollY($tomo);
	rollZ($tomo);
    };
}

sub genRollZ
{
    $headerStr = "
// Testing roll up operations on different tomos. It adds seven
// consecutive slices along the z-Axis with a random starting
// point.

";

    @mdd = ( tomo_sliced, tomo_cubed, tomo_cubed_16, tomo_cubed_64 );
    foreach $tomo (@mdd)
    {
	rollZ($tomo);
    };
}

sub genSQLRollX
{
    $tomo = "tomo";

    $headerStr = "
-- tomo_rollX.sql
--   tests rolling up tomo along x-axes.

--#SET PERF_DETAIL 1 ROWS_FETCH -1 ROWS_OUT 0
  
";
    print $headerStr;
    SQLrollX($tomo);
}

sub genSQLRollY
{
    $tomo = "tomo";

    $headerStr = "
-- tomo_rollY.sql
--   tests rolling up tomo along y-axes.

--#SET PERF_DETAIL 1 ROWS_FETCH -1 ROWS_OUT 0
  
";
    print $headerStr;
    SQLrollY($tomo);
}

sub genSQLRollZ
{
    $tomo = "tomo";

    $headerStr = "
-- tomo_rollZ.sql
--   tests rolling up tomo along z-axes.

--#SET PERF_DETAIL 1 ROWS_FETCH -1 ROWS_OUT 0
  
";
    print $headerStr;
    SQLrollZ($tomo);
}

genSQLRollX();
