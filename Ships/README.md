Game: Ships-battle

Platform: Windows 

Shape: Console 

Version: 0.1

Date: 12.06.2017

Author: Karol Bartyzel 



Configuration:
1. Download this directory.
2. Open cmd in this directory and type to compile: 
  
    -gcc -std=c99 -o player player.c -lws2_32
  
    -gcc -std=c99 -o server server.c -lws2_32
  
3. Run server without arguments
4. Run players with args as follows:

    -h (prints help)
  
    -i ip (you have to specify ip of server if you use another computer than server is estabilished on
  
    -s player_name (prints history concerning player_name)
