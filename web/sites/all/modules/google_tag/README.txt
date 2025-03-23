Google Tag Manager is a free service (registration required) to manage the
insertion of tags for capturing website analytics.

You can sign up for free at https://www.google.com/analytics/tag-manager/.

This module follows the guidelines as shown at
https://developers.google.com/tag-manager/quickstart.

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.

* Navigate to Administer >> Site building >> Modules and enable
"Google Tag Manager" in the Statistics section.

-- CONFIGURATION --

* You may want to disable other Google tracking such as the Google Analytics
module, since it will duplicate Google Tag Manager tracking.

* Visit Administer >> User management >> Permissions and check the appropriate
roles for the "administer google tag manager" permission in the google_tag
module section.

* Go to Administer >> Site configuration >> Google Tag Manager and enter your
Container ID and any other relevant settings and then Save configuration.

* Edit your active theme page template files and insert the following right
after the <body> tag:

<?php print $google_tag_body; ?>

You may have more than one page template such as page-front.tpl.php in addition
to page.tpl.php. Make sure each page template has the above snippet inserted.

* View the page source and verify the Google Tag Manager snippets are being
inserted in the <head> section and immediately after the <body> tag.

-- TROUBLESHOOTING --

* There are many ways the snippets can be excluded from the output.  Make sure
you have the correct Page paths and User roles defined at Administer >>
Site configuration >> Google Tag Manager.  Also, make sure the snippet above
in the CONFIGURATION section is inserted in all your page templates after the
<body> tag.

-- CREDITS --

* Jim Berry ("solotandem", http://drupal.org/user/240748)
* Drupal 6 port by Ed Reel (https://www.drupal.org/u/uberhacker)
