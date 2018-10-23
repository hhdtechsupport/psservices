The Federal Information Security Management Act (FISMA) is United States 
legislation that defines a comprehensive framework to protect government 
information, operations, and assets against natural or man-made threats.

This module aims to address all the FISMA requirements concerning user accounts
except those dealing with passwords; those requirements are admirably dealt
with in the Password Policy module.

Even if you are not subject to the FISMA requirements, you may wish to use this
module for the security and features it provides.

For a full description of the module, visit the project page:
  http://drupal.org/project/fisma_user

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/fisma_user


-- REQUIREMENTS --

Elements.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.

* There is some duplication between this module and the Flood Control, Inactive
  User, and Government Warning module. If you have the Government Warning 
  module enabled, this module will use its text and disable that module. The
  function of the Flood Control module is completely duplicated in this module,
  plus an additional setting that is not exposed by that module. If it is
  enabled, the Flood Control module will be disabled. This module des not 
  implement all of the features of the Inactive User module; it is up to you
  to determine whether you wish to continue using that module.


-- CONFIGURATION --

* Configure user permissions in Administration » Configuration »
  People » FISMA User Settings:

  - Use the administration pages and help (System module)

    The admin pages are tabbed by feature. Features are optional depending upon
    your FISMA level.
    
    You must have "Administer site configuration" permission.

  - There are comments and help information on each page of the settings.
    If they are not clear enough, please submit an issue to clarify them.
