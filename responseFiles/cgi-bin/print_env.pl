#!/usr/bin/perl

use strict;
use warnings;

# Get environment variables
my $request_method = $ENV{'REQUEST_METHOD'} || "";
my $content_length = $ENV{'CONTENT_LENGTH'} || "";
my $query_string   = $ENV{'QUERY_STRING'}   || "";

# Print environment
sub print_environment {
    my $output = "Environment:\n";
    $output .= "REQUEST_METHOD: $request_method\n";
    $output .= "CONTENT_LENGTH: $content_length\n";
    $output .= "QUERY_STRING: $query_string\n";

    # Calculate output length
    my $output_length = length($output);
    $output = "Content-Length: $output_length\n\n" . $output;
    return $output;
}

# Call the function to get the output string
my $output_string = print_environment();

# Print the output string
print "Content-type: text/plain\n";
print $output_string;
