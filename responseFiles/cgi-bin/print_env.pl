#!/usr/bin/perl

use strict;
use warnings;

# Get environment variables
my $request_method = $ENV{'REQUEST_METHOD'} || "";
my $content_length = $ENV{'CONTENT_LENGTH'} || "";
my $query_string   = $ENV{'QUERY_STRING'}   || "";
my $script_name = $ENV{'SCRIPT_NAME'} || "";
my $path_info = $ENV{'PATH_INFO'} || "";


if ($query_string eq "user_input=rick+roll"){
  my $html_content = "<html>\n<head>\n<title>Perl CGI Example</title>\n<meta http-equiv=\"refresh\" content=\"0; URL=https://www.youtube.com/watch?v=dQw4w9WgXcQ\">\n</head>\n<body>\n<p><a href=\"https://www.youtube.com/watch?v=dQw4w9WgXcQ\">Click here</a>!</p></body>\n</body>\n</html>";
  my $content_length = length($html_content);

  print "Content-Type: text/html\r\n";
  print "Content-Length: $content_length\r\n\r\n";

  print $html_content;
  exit (42);
}

# Print environment
sub print_environment {
    my $output = "Environment:\n";
    $output .= "REQUEST_METHOD: $request_method\n";
    if ($content_length eq ""){
      $output .= "CONTENT_LENGTH: 0\n";
    }
    else{
      $output .= "CONTENT_LENGTH: $content_length\n";
    }
    if ($query_string eq ""){
      $output .= "QUERY_STRING: empty\n";
    }
    else{
      $output .= "QUERY_STRING: $query_string\n";
    }
    $output .= "SCRIPT_NAME: $script_name\n";
    $output .= "PATH_INFO: $path_info\n";

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
