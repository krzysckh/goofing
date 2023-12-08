#!/usr/bin/perl

use strict;
use warnings;

use feature 'say';

use GUIDeFATE;
use Tk;

my $frame;
my $layout = <<_;
+--------------------------+
|T giga test               |
+M-------------------------+
| [       ]     [        ] |
|                          |
| [         ]              |
|                          |
| { sum }                  |
+--------------------------+
_

sub btn3 {
  $frame->setValue("textctrl2", $frame->getValue("textctrl0") +
    $frame->getValue("textctrl1"));
}

my $gui = GUIDeFATE->new($layout, "gtk", "a");
$frame = $gui->getFrame() || $gui;
$gui->MainLoop();
