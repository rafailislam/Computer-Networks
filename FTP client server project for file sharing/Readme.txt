Project Name: HW 3

This working porcedure is given bellow. 


From server
	make ftp_server
	./ftp_server 12000   		(./program_name <port_number>)


From client 
	make ftp_client
	./ftp_client 146.7.9.131 12000 (./program_name <server_ip> <server_port_number>)


ftp> ls server    (for showing list on server directory)

ftp> d 5          (for downloading file no 5 from server to client)

ftp> ls client    (for showing list on client directory)

ftp> u 4	  (for uploading file no 4 from client into server)

ftp> bye



* in client directory, two .txt file (client_file_list.txt and server_file_list.txt  ) will be created to store the list of files

* in server directory, one .txt file (list.txt) will be created to store the list of files