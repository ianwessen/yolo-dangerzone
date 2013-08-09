#! /usr/bin/perl
use strict;
use warnings;
use List::Util qw[min max];
my %collect = ();
foreach my $file (glob("WJetsYieldsData_ForTable_[01]j.txt")) {
    my ($j) = ($file =~ /WJetsYieldsData_ForTable(_.j)\./);
    open FILE, $file;
    my $header = <FILE>;
    $_ = <FILE>; s/^\s+//;
    my @columns = split(/\s+/, $_);
    while (<FILE>) {
        my @items = split(/\s+/, $_);
        my $mass = shift(@items);
        foreach my $i (0 .. $#columns) {
            $collect{$columns[$i].$j} .=  sprintf('%s  %s  %s', $mass, max($items[$i*3],0), $items[$i*3+2])."\n";
        }
    }
    close FILE;
}
foreach (sort(keys(%collect))) {
    my $text = $collect{$_};
    s/mu/m/g; s/el/e/g;
    open CARD, "> WJet_$_.txt";
    print CARD $text;
    close CARD;
    print "> WJet_$_.txt\n";
}

# for the 2 jet bin use only the sum
system("awk '{print \$1 \"\t\" \$14 \"\t\" \$16}' WJetsYieldsData_ForTable_2j.txt | tail -n 17 > WJet_all_2j.txt");
print "> WJet_all_2j.txt\n";
