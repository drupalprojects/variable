Readme
------

A small module which accepts an HTTP POST and saves the POST variables to the variables
table.

These variables are then available to be used elsewhere in Drupal,
such as in a custom Block. This is an easy way to export external content into Drupal.
This module requires that the FORM contain 'name' and 'pass' parameters.
These parameters must corrospond to the username and password of an active user who has
the 'administer site configuration' permission. The proper Action for the FORM is
index.php?q=variable.

For example, I have a script which periodically POSTS stats from my Napster server
into this module. These stats are shown in a custom block.

Author
------

Moshe Weitzman <weitzman@tejasa.com>

