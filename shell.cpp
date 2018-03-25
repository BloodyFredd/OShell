// C Program to design a shell in Linux
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
//#include<curses.h>
//#include<term.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
 
#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
 
// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")
 
// Greeting shell during startup
void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****OS COURSE SHELL****");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}
 
// Function to take input.
string takeInput(string str)
{
    cout<< ">" ;
    string buf;
    getline(cin,buf);
    if(cin.eof()){
	cout << " C ya!\n";
	exit(0);
    }
    if(!buf.empty()){
    if (buf.length() != 0) {
       // add_history(buf);
        str=buf;
	cout<< "\nim here, here is buf:" << buf << " here is str:" << str << "\n\n";
        return str;

    } 
    else {
        return "";
    }
    }
    return "";
}

// This function returns the length of an array string.
int lengthOfArrayString(string str[])
{
	int i = 0, length = 1;
	while(str[i] != "\0"){
		i++;
		length++;
	}
	return length;
}

// This function returns the length of a string.
int lengthOfString(string str)
{
	int i = 0, length = 1;
	while(str[i] != '\0'){
		i++;
		length++;
	}
	return length;
}

char *RepHome(const char *s, const char *oldW,const char *newW)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
 
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;
 
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
 
    // Making new string of enough length
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);
 
    i = 0;
    while (*s)
    {
        // compare the substring with the result
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
 
    result[i] = '\0';
    return result;
} 

// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    char *home;
    //getcwd(cwd, sizeof(cwd));
    home = getenv("HOME");
    char *tmp = new char[1024];
    strcpy(tmp, getcwd(cwd, sizeof(cwd)));
    tmp = RepHome(getcwd(cwd, sizeof(cwd)),home,"~");
    cout<<"\nOS Shell Ex2: "<< tmp;
}
 
/*
// Function where the system command is executed
void execArgs(string parsed[])
{
    // Forking a child
    pid_t pid = fork(); 
    int length = lengthOfArrayString(parsed);
 	    cout << "\nBefore the func, parsed0: " <<parsed[0];
	    cout << "parsed:" <<parsed;
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
	cout<<"im in pid=0\n";
	const char * c1=parsed[1].c_str();
	char ** c2;
	int i = 0, k = 0;
	while(i < length){
		cout<<"semek\n\n";
		while(k < lengthOfString(parsed[i])){	
			c2[i][k] = parsed[i][k];
			k++;
		}
		i++;
	}
	i=0, k=0;
	while(i < length){
		cout<<"\nstr: ";   
		while(k < lengthOfString(parsed[i])){	
			cout<<c2[i][k];
			k++;
		}
		i++;
	}
        if (execvp(c1, c2) < 0) {
            printf("\nCould not execute command..");
	    cout << parsed[0];
	    cout << parsed;
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL); 
        return;
    }
}
*/
 
// Function where the piped system commands is executed
void execArgsPiped(string parsed[], string parsedpipe[])
{/*
    // 0 is read end, 1 is write end
    int pipefd[2]; 
    pid_t p1, p2;
 
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }
 
    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
 
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();
 
        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }
 
        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
*/
}
 
// Help command builtin
void openHelp()
{
    puts("\n***WELCOME TO THE SHELL HELP***"
        "\nList of Commands supported:"
        "\n>cd"
        "\n>ls"
        "\n>exit"
        "\n>all other general commands available in UNIX shell"
 );
    return;
}
 
char **Cut_Last(char** parsed, int *length)
{
  char **temp=new char *[(*length)-1];
  int i=0;
  while(i<((*length)-1))
  {
	cout<< "\nrun\n";
	temp[i]=new char[strlen(parsed[i])];
	strcpy(temp[i],parsed[i]);
	i++;
  }
  //*length--;
  return temp;
  cout<<"args0: "<< temp[0];
  cout<<"args1: "<< temp[1];
}

// This function kills a zombie.
int killZombie(int stat)
{
	pid_t status;
	int es = 0;
	status = waitpid(-1, &stat, WNOHANG);
	if(WIFEXITED(stat))
		es=WEXITSTATUS(stat);
	if(status > 0)
		cout << "[" << status << "] : exited, status = " << es << "\n";
	return 0;
}

char* CheckVar(char* str)
{
   int i=0, len=strlen(str),CheckExit=0, es, res;
   string s;
   string Ostring;
   char *newstr;
   char *newOriginalstr;
   char *var;
   while(i<len)
   {
	if(str[i+1]!='\0')
	{
	   if(str[i]=='$' && str[i+1]!='$' && str[i+1]!='/')
	   {
		Ostring+=str[i];
		i++;
	 	while(str[i]!='$' && str[i]!='/' && str[i]!='\0')
		{
			s+=str[i];
			Ostring+=str[i];
			if(Ostring=="$?")
				CheckExit=1;
			i++;		
		}
		newstr=new char[s.length()+1];
		newOriginalstr=new char[Ostring.length()+1];
		strcpy(newstr,const_cast<char*>(s.c_str()));
		strcpy(newOriginalstr,const_cast<char*>(Ostring.c_str()));
		cout<<"the newstr:  "<<newstr<<"\n";
		cout<<"the newOriginalstr:  "<<newOriginalstr<<"\n";
		if(CheckExit==0)
		{
		   var=getenv(newstr);
		   if(var != NULL)
		   	str=RepHome(str,newOriginalstr,var);	
		    else cout << "No such variable.\n";	
		}
		else
		{
		    es=system("./shell.cpp");
		    res=(WEXITSTATUS(es));
		    var=new char[1024];
		    sprintf(var, "%d" , res);
		    str=RepHome(str,newOriginalstr,var);	
		}		
		if(str[i]=='$')
			i--;
	   }
	}
	s="";
	Ostring="";
	i++;
   }

   return str;
}

// This function gets the shell command and works accordingly.
int shellCom(char **args, int length)
{
 pid_t pid;
 int status,cond=0;
  cout<<"the last arg is:" << args[length - 1][0] <<" sup";
 if(args[length - 1][0]=='&')
	{
	   cout<<"yesss it is right!!";
	   cond=1;
	   args=Cut_Last(args,&length);
	   length--;
	}
 /*for(int j=0; j<length ; j++)
 {

   if(strstr(args[j],"~")!=NULL)
    {
	cout<<"Found it! " ;
	args[j]=RepHome(args[j],"~","$HOME");
	cout<<"after i Found it! " << args[j];
   }
   if(args[j][0]=='$')
   {	
	if(strstr(args[j],"$HOME")!=NULL)
	{	
		cout<<"  \nEntered\n";  
		var=getenv("HOME");
		cout<<"The var is:  " << var<< "\n\n\n";
		args[j]=RepHome(args[j],"HOME",var);
	}
	else
	{
	cout<<args[j]++;
	var=getenv(args[j]);
	if(!var)
	  {
	   cout<< "not a variable! but here it is  "<<var <<"\n";
	  }
	else
	  {
	    args[j]=new char[strlen(var)];
       	    strcpy(args[j],var);
	    
	  }	
	}
   }
 }*/
 pid = fork(); 
 if(pid == 0)
 {
   if(execvp(args[0],args) == -1)
   {
      perror("error here");
   }
   exit(EXIT_FAILURE);
 }
 else if(pid<0) 
 {
    perror("lsh");
 }
 else if(cond==0) 
 {
    do 
    {
         waitpid(pid, &status , WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
 }
 else
     cout<<"\n["<<pid<<"]";

 return 1;

}


// Function to execute builtin commands
int ownCmdHandler(char** parsed, int *length)
{

    int NoOfOwnCmds = 6, i, switchOwnArg = 0, succ=0,stat=0,res;
    char** ListOfOwnCmds = new char*[NoOfOwnCmds];
    for (i = 0; i < NoOfOwnCmds; i++) 
	ListOfOwnCmds[i] = new char[10];
    strcpy(ListOfOwnCmds[0], "exit");
    strcpy(ListOfOwnCmds[1], "cd");
    strcpy(ListOfOwnCmds[2], "help");
    strcpy(ListOfOwnCmds[3], "hello");
    strcpy(ListOfOwnCmds[4], "kill");
    strcpy(ListOfOwnCmds[5], "other");
    char* username;
    char* var;
    for (i = 0; i < NoOfOwnCmds; i++) {
	cout << "The parse: " << parsed[0] << "\n";
	cout << "The list: " << ListOfOwnCmds[i] << "\n";

        if (!strcmp(parsed[0], ListOfOwnCmds[i])) {
            switchOwnArg = i + 1;
	    succ=1;
            break;
        }

	if(parsed[0][0]=='c' && parsed[0][1]=='d')
	{
		succ=1;
		switchOwnArg=2;
		break;	
	}	
    }

 for(int j=0; j<(*length) ; j++)
 {
   if(strstr(parsed[j],"~")!=NULL)
    {
	var=getenv("HOME");
	parsed[j]=RepHome(parsed[j],"~",var);
   }

   parsed[j]=CheckVar(parsed[j]);
  /* if(args[j][0]=='$')
   {	
	if(strstr(args[j],"$HOME")!=NULL)
	{	
		cout<<"  \nEntered\n";  
		var=getenv("HOME");
		cout<<"The var is:  " << var<< "\n\n\n";
		args[j]=RepHome(args[j],"HOME",var);
	}
	else
	{
	cout<<args[j]++;
	var=getenv(args[j]);
	if(!var)
	  {
	   cout<< "not a variable! but here it is  "<<var <<"\n";
	  }
	else
	  {
	    args[j]=new char[strlen(var)];
       	    strcpy(args[j],var);
	    
	  }	
	}
   }*/
 }


   if(switchOwnArg==5)
   	stat=(atoi(parsed[1]));

   if(succ==0)
   {
	switchOwnArg=6;

   }

    switch (switchOwnArg) {
    case 1:
        printf("\nC ya!\n");
        exit(0);
    case 2:
        res=chdir(parsed[1]);
	if(res==-1)
	   cout<<"cd: No such file of directory";
        return 1;
    case 3:
        openHelp();
        return 1;
    case 4:
        username = getenv("USER");
        printf("\nHello %s.\nMind that this is "
            "not a place to play around."
            "\nUse help to know more..\n",
            username);
        return 1;
    case 5:
	killZombie(stat);
	return 1;
    case 6:
	 shellCom(parsed, *length);
	 return 1;
    default:
        break;
    }
 
    return 0;
}
 
// function for finding pipe
int parsePipe(string str, char** strpiped)
{
   /* int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }
 
    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    } */
   return 0;
}
 
// function for parsing command words
char ** parseSpace(string str, char **parsed, int *length)
{
	string s;
	int i = 0, k = 0, count=0,len=str.length();
	for(i=0; i<=len ; i++)
	{
	    if(str[i] == ' ' || str[i] == '\0')
		count++;
	}
	cout << "\nI am count!" << count <<"\n";
	*length = count;
	parsed=new char *[count+1];
	i=0;
cout << "\nI am len!" << lengthOfString(str) <<"\n";
   while(k <= lengthOfString(str)){
	  
	if(str[k] == ' ' || str[k]=='\0')
	{
 	
		s += '\0';
		parsed[i]=new char[s.length()];
		strcpy(parsed[i],const_cast<char*>(s.c_str()));
		s="";
		i++;
		k++; 
	}

	//if(str[k]!= '\0')
	    s += str[k];
	cout << "I am " << s << "\n";
	k++;
    }

     //parsed[i] = s;
    cout<< "\nparsed0: " <<parsed[0]<<"\n";
   // cout<< "parsed1: " <<parsed[1]<<"\n";
   return parsed;
}
 
// This function takes a string and splits it.
int processString(string str, char **parsed/*, char** parsedpipe*/)
{
 
    char* strpiped[2];
    int piped = 0, length = 0;
    piped = parsePipe(str, strpiped);
     
    if (piped) {
        //parseSpace(strpiped[0], parsed);
        //parseSpace(strpiped[1], parsedpipe);
 
    } else {
        parsed=parseSpace(str, parsed, &length);

    }
    if (ownCmdHandler(parsed, &length))
        return 0;
    else
        return 1 + piped;
}
 
// This is out main function.
int main()
{
    string inputString;
    char* parsedArgs[MAXLIST];

    //init_shell();
 
    while (1) {
        // print shell line
        printDir();
        // take input

	inputString=takeInput(inputString);
	cout<<"here i am: " << inputString << "end\n";        
	if (inputString=="")
            continue;
       // process
            processString(inputString, parsedArgs/*, parsedArgsPiped*/);
    }
    return 0;
}
