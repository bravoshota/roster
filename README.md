# roster
download contacts from URL, load and visualize it. also add sorting, filtering, etc.

Roster download url and output file for downloaded contacts are stored in config file (config.json: "URL", "rosterFile").

On start the app gets cached contact file (as indicated into config file) and loads it into table view control.
If there is no cashed file found then GUI is empty while user doesn't fetch data manually. Fetching can be done even if there is already cached file (if user finds reasonable to download it again).

Model/view architecture of the table view gives possibility to lazy load data. The page size is 64. More data is loaded by scroll request.
Contacts in table view are sorted by group ids and inside groups there are sorted alphabetically (Name + surname).

The table view displays only avatar, firstname, lastname and group information about contacts. In order to view all the details user double-clicks a particular contact and dialog appears with big avatar and all the details.

Also filtering is available: user inputs the text in line edit control and the results are filtered (again by lazy load principle).
Searching is made into first and last names of contacts, no case sensitive.

TIPS:
- there may be some bugs but I tried more or less make the app stable.
- icons are created and stored in model's structure upon request to avoid rendering of all icons at the beginning.
- config.json is part of repository and is installed (with default params) during the build.
- user can watch download progress by appeared progress dialog and cancel it by request.
- downloading process is non-blocking and user can continue viewing contacts.
- installing config file is implemented by qmake; The name of the config is used in the code as defined in qmake.

Regards,
Shota Meladze
