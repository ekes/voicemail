Adds voicemail onto a VoIP extension; primarily a node extension.

Two modules?

Just to seperate the core voicemail on an extension and the node related functions. 
The former could just be an include in the latter to keep module numbers down;
unless someone wants to make a voicemail on an extension which is not a node.

TODO

 * Permissions. What would the obvious set of permissions be. It's not node_access('view').
 * Views:
   * Make the available mailboxes a dropdown (it's free text at the moment).
 * Listen to message on the phone - the script is in views - but need to
     integrate with authentication.
