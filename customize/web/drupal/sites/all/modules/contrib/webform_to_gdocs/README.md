# Webform to GDocs

## Installation

1. Go to [console.developers.google.com](https://console.developers.google.com) and create a new project.
   a. Give it a name.
   b. Enable the Drive API under "APIs & auth" » "APIs".
   c. Create a new `OAuth client ID` under "APIs & auth" » "Credentials"
      i. The type is `Web Application`
      ii. The origin is your full URL, e.g. `https://www.example.com`
      iii. The authorized redirect is `[the full URL from step ii]/webform_to_gdocs/oauth2callback` with a newline at the end.
      iv. Note that it may take several minutes before your redirect URL propagates across Google's network in order for the next step to work.
2. On the admin page (`admin/config/content/webform/gdocs`), enter the client details that you were given in step iii.
3. Click the authorize button and follow the instructions to authorize your site.
