#!/usr/bin/env perl

use v5.14;

use strict;
use warnings;

use GD::Graph::hbars;
use GD::Graph::bars;
use File::Basename qw(basename);
use Sort::Hash qw(sort_hash);
use Time::Piece;

use Class::Struct;
use constant {
  e_skip   => 'SKIP',
  e_eof    => 'EOF',
  e_load   => 'LOAD',
  min_time => 30,
  font     => '/home/kpm/.fonts/ttf/Lilex-Regular.ttf'
};

struct(Elem => {
  type  => '$',
  stamp => '$',
  value => '$'
});

my %tmap = (
  'SKIP' => e_skip,
  'EOF'  => e_eof,
  'LOAD' => e_load
);

open my $hist, '<', $ENV{"HOME"} . "/.mpvq_history";
my (@el, %listened, %skipped);
my %time = (0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,12,0,13,0,14,0,15,
            0,16,0,17,0,18,0,19,0,20,0,21,0,22,0,23,0);

while (<$hist>) {
  /^(\d+)\s(\w+)\s(.*)$/;
  push @el, Elem->new(type => $2, stamp => $1, value => $3);
}
close $hist;

for (0..(@el - 1)) {
  $time{localtime($el[$_]->stamp)->strftime('%H')}++;
  if ($el[$_]->type eq e_eof) {
    $listened{basename $el[$_]->value}++;
  } elsif ($el[$_]->type eq e_load) {
    if (defined $el[$_+1] and $el[$_+1]->type eq e_skip
        and ($el[$_+1]->stamp - $el[$_]->stamp < min_time)) {
      $skipped{basename $el[$_]->value}++;
    } else {
      $listened{basename $el[$_]->value}++;
    }
  }
}

sort_hash(\%listened);

my $lg = GD::Graph::hbars->new(1000, 1000);
$lg->set_title_font(font, 12);
$lg->set_x_axis_font(font, 8);

my $tg = GD::Graph::bars->new(1000, 1000);
$tg->set_title_font(font, 12);
$tg->set_x_axis_font(font, 8);

my @ldata = (
  [sort_hash 'desc', \%listened],
  [sort { $b <=> $a } values %listened],
);

my @tdata = (
  [sort_hash 'desc', \%time],
  [sort { $b <=> $a } values %time],
); # TODO: not that cringe.

open my $lf, '>', "listened.png";
open my $tf, '>', "time.png";

print $lf $lg->plot(\@ldata)->png;
print $tf $tg->plot(\@tdata)->gif;

close $lf;
close $tf;
