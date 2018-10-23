The Computed Field Order module helps to deal with the situation of fields
being computed in the order in which they are found in the "field_config_instance"
database table. Since that table is largely populated in the order in which you
add fields to an entity, one may control the order of computation by adding
fields in the order desired. Occasionally, however, it may be necessary to 
have a later-added field computed before an existing field, such as when a field
containing data must be deleted and re-added. The easiest solution to this is to
delete and redefine the subsequent fields, thereby preserving the database integrity.
But if there are a lot of uses of that field and re-saving the content is difficult,
then this module may help you restore the desired computation order.

If there is any way to take the manual solution above, we reccommend doing so.
You are strongly urged to back up your database before using this module.
We also suggest testing your site after using it, particularly the fields you
are altering.

* Install and enable the module in the usual manner.
* Back up you database (optional, but reccommended).
* Go to admin/config/content/computed_field_order
  (Configuration >> Content authoring >> Computed Field Order).
* Click on the bundle type that needs to be changed.
* Drag the field handles to the order you want.
* Click on the "Change order" button.
* Test to make sure the correct effect has occurred.

You may disable the module when it is not in use so that it
is not loaded at every page build.
