//********************************************************************
//
// Rafail Islam
// Computer Networks
// Assignment #5
// November 21, 2019
// Instructor: Dr. Ajay K. Katangur
//
//********************************************************************

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>

//********************************************************************
//
// Global variables
// ip_addar_dot     char    Takes dotted decimal ip address from command line
// subnet_mask      char    Takes dotted decimal Subnet mask from command line
// mask_byte        int     Holds decimal equivalent for each byte of subnet mask
// ip_byte          int     Holds decimal equivalent for each byte of ip address
//
//********************************************************************
char ip_addar_dot[16],subnet_mask[16];
int mask_byte[4], ip_byte[4];

//********************************************************************
//
// DNS Display function
//
// This function shows the DNS name of a given IP address.
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// ip_addr_dot		char		The IP address stored in char array
//
// Local Variables
// ---------------
// *hostPtr		hostent	    This structure is used to call host name
// addrPtr   	in_addr		To get the ip of returned host
// ip_addr_long u_long      Convert dotted decimal into Internet binary format
//
//
//*******************************************************************

void displayDNS(char ip_addr_dot[])
{
    struct hostent *hostPtr;
    struct in_addr *addrPtr;
    u_long ip_addr_long;
    char* addr_ptr;
    ip_addr_long = inet_addr(ip_addr_dot);
    addr_ptr = (char *) &ip_addr_long;
    hostPtr = gethostbyaddr(addr_ptr, 4, AF_INET);
    if (hostPtr == NULL)
        printf("Host %s not found\n", ip_addr_dot);
    else
    {
        addrPtr = (struct in_addr *) *hostPtr->h_addr_list;
        printf("The DNS name of %s : %s\n", inet_ntoa(*addrPtr),hostPtr->h_name);
    }
}


//********************************************************************
//
// Function ipRange
//
// This function calculate the appropriate host range for any specific subnet
//
// Return Value
// ------------
// int                      Return range for host IP
//
// Value Parameters
// ----------------
// potion		int		    Option contains byte of subnet mask
//
// Local Variables
// ---------------
// none
//
//*******************************************************************
int ipRange(int option)
{
    switch(option)
    {
    case 0:
        return 255;
    case 128:
        return 127;
    case 192:
        return 63;
    case 224:
        return 31;
    case 240:
        return 15;
    case 248:
        return 7;
    case 252:
        return 3;
    case 254:
        return 1;
    default:
        printf("Not space for ip address\n");
        return 0;

    }
}

//********************************************************************
//
// Function loop1
//
// loop1 function uses  1 for loop to iterate through last byte of host id
//
// Return Value
// ------------
// void
//
// Local Variables
// ---------------
// ip       char	    Holds sting of ip address
// i      	int		    Loop iterators
// first_ip int         First IP of the subnet
// range    int         Range of the subnet
//
//*******************************************************************
void loop1()
{
    int i,range;
    char ip[16];

    // Doing bitwise AND for getting first ip of this subnet
    int first_ip = ip_byte[3] & mask_byte[3];
    // Getting range of this subnet
    range = ipRange(mask_byte[3]);

    for(i = first_ip + 1 ; i < first_ip + range ; i++)
    {
        sprintf(ip,"%d.%d.%d.%d",ip_byte[0],ip_byte[1],ip_byte[2],i);

        // Displaying DNS name for associated ip address
        displayDNS(ip);
        //printf("%s\n",ip);
    }

}
//********************************************************************
//
// Function loop2
//
// loop2 function uses  2 for loop to iterate through 2 bytes of host id
//
// Return Value
// ------------
// void
//
// Local Variables
// ---------------
// ip       char	    Holds sting of ip address
// i,j   	int		    Loop iterators
// first_ip int         First IP of the subnet
// range    int         Range of the subnet
//
//*******************************************************************
void loop2()
{
    int i,j,range;
    char ip[16];
    // Doing bitwise AND for getting first ip of this subnet
    int first_ip = ip_byte[2] & mask_byte[2];
    // Getting range of this subnet
    range = ipRange(mask_byte[2]);
    for(i = first_ip; i < first_ip+ range+1; i++)
        for(j=1; j<255; j++)
        {
            sprintf(ip,"%d.%d.%d.%d",ip_byte[0],ip_byte[1],i,j);

            // Displaying DNS name for associated ip address
            displayDNS(ip);
            //printf("%s\n",ip);
        }

}
//********************************************************************
//
// Function loop3
//
// loop3 function uses  3 for loop to iterate through 3 bytes of host id
//
// Return Value
// ------------
// void
//
// Local Variables
// ---------------
// ip       char	    Holds sting of ip address
// i,j,k   	int		    Loop iterators
// first_ip int         First IP of the subnet
// range    int         Range of the subnet
//*******************************************************************

void loop3()
{
    int i,j,k,range;
    char ip[16];
    // Doing bitwise AND for getting first ip of this subnet
    int first_ip = ip_byte[1] & mask_byte[1];
    // Getting range of this subnet
    range = ipRange(mask_byte[1]);
    for(i = first_ip; i < first_ip+ range+1 ; i++)
        for(j=0; j<256; j++)
            for(k=1; k<255; k++)
            {
                sprintf(ip,"%d.%d.%d.%d",ip_byte[0],i,j,k);

                // Displaying DNS name for associated ip address
                displayDNS(ip);
                //printf("%s\n",ip);
            }

}

//********************************************************************
//
// Function loop4
//
// loop4 function uses  4 for loop to iterate through 4 bytes of host id
//
// Return Value
// ------------
// void
//
// Local Variables
// ---------------
// ip           char	    Holds sting of ip address
// i,j,k,l   	int		    Loop iterators
// first_ip     int         First IP of the subnet
// range        int         Range of the subnet
//
//*******************************************************************
void loop4()
{
    int i,j,k,l,range;
    char ip[16];
    // Doing bitwise AND for getting first ip of this subnet
    int first_ip = ip_byte[0] & mask_byte[0];
    // Getting range of this subnet
    range = ipRange(mask_byte[0]);
    for(i = first_ip; i < first_ip+ range + 1 ; i++)
        for(j=0; j<256; j++)
            for(k = 0; k < 256; k++)
                for(l = 1; l<255; l++)
                {
                    sprintf(ip,"%d.%d.%d.%d",i,j,k,l);

                    // Displaying DNS name for associated ip address
                    displayDNS(ip);
                    //printf("%s\n",ip);
                }

}
//********************************************************************
//
// Function isValid
//
// isValid function takes one byte from subnet mask and check whether it is valid or not
//
// Return Value
// ------------
// bool                 Return true if the byte from subnet mask is valid otherwise return false
//
// Value Parameters
// ----------------
// 	a       int     	takes one byte from subnet mask
//
// Local Variables
// ---------------
// none
//
//*******************************************************************
bool isValid(int a)
{
    if (a == 0 || a == 128 || a == 192 || a==224 || a == 240 || a == 252 || a == 254 || a ==255)
        return true;
    else
        return false;
}
//********************************************************************
//
// Main function
//
// This function takes parameter from command line as subnet mask and ip address. Then show the DNS name for the available ip address
//
// Return Value
// ------------
// int
//
// Value Parameters
// ----------------
// 	**argv	char	takes subnet mask and ip address
//
// Local Variables
// ---------------
// i  	        int		    Loop iterators
// loc          int         Used to detect the prefix bytes of IP address and to check the validity of subnet mask
// flag         int         Used to check the validity of subnet mask
// suffix       int         Denotes suffix byte(s) of ip address
//*******************************************************************
int main(int argc, char **argv)
{

    if(argc != 3)
    {
        printf("Invalid Number of Arguments\n");
        return 0;
    }
    sscanf(argv[1], "%s",subnet_mask);
    sscanf(argv[2], "%s",ip_addar_dot);


    //strcpy(subnet_mask,"252.255.253.0");
    //strcpy(ip_addar_dot,"192.168.44.55");

    // Extract bytes from subnet mask and copy into mask_byte[] as a decimal number
    sscanf(subnet_mask,"%d.%d.%d.%d",&mask_byte[0],&mask_byte[1],&mask_byte[2],&mask_byte[3]);

    // Check if bytes of subnet mask are valid
    if( !(isValid(mask_byte[0]) && isValid(mask_byte[1]) && isValid(mask_byte[2]) && isValid(mask_byte[3])) )
    {
        printf("Invalid subnet mask\n");
        return 0;
    }
    int i,loc,flag = 0, suffix;

    // Finding the bytes: prefix and suffix (net id and host id)
    for(i = 0; i<4; i++)
    {
        if(mask_byte[i]==255 && flag == 0)
            continue;
        else if (flag == 0 )
        {
            if(mask_byte[i]<255)
            {
                loc = i;
                flag = 1;
            }
        }
        else if(mask_byte[i] == 0)
            continue;
        else
        {
            printf("Invalid subnet mask\n");
            return 0;
        }
    }

    suffix = 4-loc;
    // Extract bytes from ip address and copy into ip_byte[] as a decimal number
    sscanf(ip_addar_dot,"%d.%d.%d.%d",&ip_byte[0],&ip_byte[1],&ip_byte[2],&ip_byte[3]);

    // Check the bytes of ip address in range ( 0 - 255 )
    if(ip_byte[0]>255 ||ip_byte[0] <0 ||ip_byte[1]>255 || ip_byte[1]<0 || ip_byte[2]>255 || ip_byte[2] <0 || ip_byte[3]>255 || ip_byte[3] <0)
    {
        printf("Invalid IP\n");
        return 0;
    }

    // Check and call the appropriate function for the subnet mask to show all ip address
    switch(suffix)
    {
    case 1:
        loop1();
        break;
    case 2:
        loop2();
        break;
    case 3:
        loop3();
        break;
    case 4:
        loop4();
        break;
    default:
        printf("Error in subnet mask\n");
    }


    return 0;
}
