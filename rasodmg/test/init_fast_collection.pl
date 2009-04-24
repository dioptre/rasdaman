#!/usr/bin/perl -w

my $COLLECTION="orthorgb";
my $SERVER="zombie";
my $YMIN=0;
my $YMAX=4999;
my $XMIN=0;
my $XMAX=4999;
my $SCALELEVELS=8;
my $IMGTYPE="RGB";#or "GREY"
my $STORAGEFORMAT="Array";
my $TRANSFERFORMAT="Array";
my $transferWidth=2000;
$transferWidth=int($transferWidth/(2**$SCALELEVELS))*(2**$SCALELEVELS);
print "#transfer width $transferWidth\n";
my $transferHeight=2000;
$transferHeight=int($transferHeight/(2**$SCALELEVELS))*(2**$SCALELEVELS);
print "#transfer height $transferHeight\n";
my $i=0;
my $currentCollection=$COLLECTION;
my $currentXMin=$XMIN;
my $currentYMin=$YMIN;
my $currentXMax=$XMIN + $transferWidth;
$currentXMax=min($XMAX, $currentXMax);
my $currentYMax=$YMIN + $transferHeight;
$currentYMax=min($YMAX, $currentYMax);
my $colltype;
my $mddtype;
my $conversiontype;
my $scaleLevel=2;
my $command;
my $scaleString;
my $domain;

sub max {
	my $currentMax = shift;
	my $next = shift;
	if ($currentMax > $next)
		{
		return $currentMax;
		}
	else	{
		return $next;
		}
	};

sub min {
	my $currentMax = shift;
	my $next = shift;
	if ($currentMax > $next)
		{
		return $next;
		}
	else	{
		return $currentMax;
		}
	};

if ($IMGTYPE eq "RGB")
	{
	$colltype="RGBSet";
	$mddtype="RGBImage";
	$conversiontype="RGBPixel";
	}
else	{
	if ($IMGTYPE eq "RGB")
		{
		$colltype="GreySet";
		$mddtype="GreyImage";
		$conversiontype="char";
		}
	else	{
		die "Unknown image type: $IMGTYPE\n";
		}
	}
for ($i=1; $i < $SCALELEVELS + 1; $i++)
	{
	$currentCollection = $COLLECTION . "_" . int($i);
	$scaleString .= "$currentCollection:$scaleLevel";
	if ($i != $SCALELEVELS)
		{
		$scaleString .= ";";
		}
	$scaleLevel *= 2;
	}
while ($currentXMax <= $XMAX)
	{
	while ($currentYMax <= $YMAX)
		{
		$domain = "[$currentXMin:" . ($currentXMax - 1) . ",$currentYMin:" . ($currentYMax - 1) . "]";
		$command = "system_update -s $SERVER --collection $COLLECTION -t --colltype $colltype --mddtype $mddtype --storageformat $STORAGEFORMAT --transferformat $TRANSFERFORMAT --scalelevels \"$scaleString\" --mdddomain \"$domain\"";
		print "$command\n";
		if ($currentYMax == $YMAX)
			{
			last;
			}
		$currentYMax += $transferHeight;
		$currentYMax = min($currentYMax, $YMAX);
		$currentYMin += $transferHeight;
		};
	if ($currentXMax == $XMAX)
		{
		last;
		}
	$currentXMin += $transferWidth;
	$currentXMax += $transferWidth;
	$currentXMax = min($currentXMax, $XMAX);
	$currentYMin = $YMIN;
	$currentYMax = $YMIN + $transferHeight;
	$currentYMax = min($YMAX, $currentYMax);
	};
