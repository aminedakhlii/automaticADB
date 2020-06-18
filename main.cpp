/* Copyright (c) 2018 Amine Dakhli <amine.dakhli@medtech.tn>
* All rights reserved.
*
* Permission is hereby granted, free of charge, to any
* person obtaining a copy of this software and associated
* documentation files (the "Software"), to deal in the
* Software without restriction, including without
* limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice
* shall be included in all copies or substantial portions
* of the Software.
*
* THE SOFTWARE IS MADE FOR EDUCATIONAL PURPOSES AND AUTOMATION OF A NUMBER OF TASKS
* AND ANY
*
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;


string get_cmd_response(string cmd){
    string response ; char buffer[128] ;
    FILE * pipe = popen(cmd.c_str(),"r") ;
    if (!pipe) return "failed to execute" ;
    while (!feof(pipe)) {
        if (fgets(buffer , 128 , pipe) != NULL){
            response += buffer ;
               }
    }
    pclose(pipe) ;
    return response ;
}
void execute_adb(string cmd){
    string fullcmd = "adb " + cmd ;
    system(fullcmd.c_str()) ;
}
int main()
{
   string wallpaper = "****************************************************************\n"
                      "******************* NA9AR LKHACHAB **************************** \n"
                      "**************************************************************** \n"
                      "OPTIONS LIST \n (1): connect to a device \n (2): tap on device's screen \n"
                      " (3): pull files from device\n (4): push files to the device\n (5): type a message: \n (6): change the volume: \n "
                      "(7) open the camera\n (8) call a number\n (9): \n (10): open phone if you know pattern or password \n "
                      "(11): open an app \n ";
   cout << wallpaper << endl ;
   while(1) {
       int order ;
       printf("enter option's number : ");
       scanf("%i",&order) ;
       cout << order ;
       switch (order) {
       case 1:
          { string ipaddr ;
           cout << "enter target ipAddr : " ;
           cin >> ipaddr ;
           execute_adb("connect "+ ipaddr);
           continue ;}
       case 2:
          { string mouse1 , mouse2 ;
           cout << "select dimentions to tap in: example : 200 120\n" ;
           cin >> mouse1 ;
           cin >> mouse2 ;
           execute_adb("shell input mouse tap "+ mouse1 +" "+ mouse2);
            continue ;}
        case 3:
       {    cout << "enter file path ex: /sdcard/..." ;
           string path ;
           cin >> path ;
           execute_adb("pull "+path);
           continue ;}

       case 4:
       { cout << "enter the target path:";
         string path;
         string file_path;
         cin>> path;
         cout << (int)' ';
         cout << "enter file path from this computer:";
         cin>> file_path;
         execute_adb("push "+file_path +" "+path);
       continue;
      }
       case 5:{
           string message ;
           cout << "type a message\n";
           cin >> message ;
           int length = message.length();
           char message_array[length + 1];
           int message_numbers[length +1] ;
           strcpy(message_array,message.c_str()) ;
           for (int i=0 ; i<length ; i++){
               if((int)message_array[i] >64 && (int)message_array[i] < 91 ){
                   int letter = (int)message_array[i] - 36 ;
                   message_numbers[i] = letter ;

               }
               else if((int)message_array[i] >96 && (int)message_array[i] < 122 ){
                   int letter = (int)message_array[i] - 68 ;
                   message_numbers[i] = letter ;

               }
               else if((int)message_array[i] == 95){
                   message_numbers[i] = 62 ;
               }
           }
           for (int i=0 ; i<length ; i++ ){
               execute_adb("shell input keyevent " + to_string(message_numbers[i]));
           }
           execute_adb("shell input keyevent 66");
           continue;

       }
       case 6:
       {
           string decision;
           int dec;
           cout<< "enter up if you want the volume up enter down if you want the volume down:\n";
           cin >> decision;
           if (decision == "down")
               dec = 25;
           else
               dec= 24;

           execute_adb("shell input keyevent "+ to_string(dec));
            continue;
       }
       case 7:
       {
           int c;
          execute_adb("shell am start -a android.media.action.IMAGE_CAPTURE");
          cout<< "when you want to take a picture type 1";
          cin>> c;
          if (c==1)
             execute_adb("shell input keyevent 66");
           continue;
       }
        case 8:
       {
           string number;
           cout<< "enter the number you want to call:";
           cin>> number;
           execute_adb("shell am start -a android.intent.action.CALL -d tel:" + number);

           continue;
       }
        case 9:
       {
         string number;
        cout<<"enter the number you want to send a message to:";
        cin>> number;




       }
        case 10:
       {
           cout << "if password type 1 ; if pattern type 2 ?";
           int choice ;
           cin >> choice ;
           if (choice == 1){
               cout << "type password : " << endl ;
               string password ;
               cin >> password ;
               execute_adb(" shell input keyevent 26");
               execute_adb(" shell input touchscreen swipe 930 880 930 380");
               execute_adb(" shell input text "+password );
           }
           else if(choice == 2){
               cout << "type pattern with numbers with : \n 1  2  3 \n 4  5  6 \n 7  8  9\n" ;
               string pattern ;
               cin >> pattern ;
               execute_adb(" shell locksettings clear --old " + pattern );
               execute_adb(" shell input keyevent 26");
               execute_adb(" shell input keyevent 26");
               execute_adb(" shell input keyevent 26");
               execute_adb(" shell input touchscreen swipe 930 880 930 380");
               execute_adb(" shell locksettings set-pattern "+ pattern);
           }
           continue ;
       }
        case 11:
       {
           string app_name;
           cout << "enter name of the app you want to open: \n" ;
           cin >> app_name ;
           string package_name = get_cmd_response("adb shell pm list packages -f "+ app_name);
           size_t pos = package_name.find("apk=");
           string pkg = package_name.substr(pos + 4 ) ;
           size_t pos2 = pkg.find("\n") ;
           pkg = pkg.substr(0, pos2) ;
           cout << pkg ;
           execute_adb("shell monkey -p " + pkg +" -c android.intent.category.LAUNCHER 1") ;
          // execute_adb("shell monkey -p " + pkg +"  -c android.intent.category.LAUNCHER 1");
           continue ;
       }

       return 0 ;// 60 169

}}
}

