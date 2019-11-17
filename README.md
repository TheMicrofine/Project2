# NetworkProject2
You may need to create a new connection in MySQL Workbench for this :
	hostname = localhost
	username = root
	password = root
	port = 3306

Open Project2.sql in workbench, set project_2 as start up database by double clicking on its name on the left sidebar.
Use the Reconnect to DBMS button and enter 'root' as the password.
Then execute the query to make the database.
Build project in Release x64.
Run without debugging to start the Authentication server.
Open 2 or more console windows, one for the chat server and the other(s) for clients.
Project2/x64/Release
Run Server.exe.
Run Client.exe.
Clients are now asked at the start if they want to login or register.
Enter 'Login' or 'Register' to continue.
Enter your credentials to continue to the chat room.
Chat room functionality works as it did from NetworkProject1.



# NetworkProject1
Build project in Debug in x86.
Open up a console window to the Debug folder.
Run Server.exe.
Open 1 or more console windows to use as clients.
Run Client.exe from the same Debug folder.
On start-up, the client will be prompted for a name and then what room they wish to join.
Enter your name and room number and you will be placed into that room.
Any messages entered will be sent to any other client in the same room.
To leave a room enter 'LeaveRoom'.
Leaving a room will bring the client back to select another room.
