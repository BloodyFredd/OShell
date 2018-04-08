// CPP Program to design a shell in Linux
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;

#define MAXLIST 100 // max number of commands to be supported
int ExitCode=0;
// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")
 
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
        str=buf;
        return str;

    } 
    else {
        return "";
    }
    }
    return "";
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

// Get the current directory.
char *RepHome(const char *s, const char *oldW,const char *newW)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
 
    // Counting the number of times old word occur in the string.
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;
 
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
 
    // Making new string of enough length.
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);
 
    i = 0;
    while (*s)
    {
        // Compare the substring with the result.
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

// Function to print current directory.
void printDir()
{
    char cwd[1024];
    char *home;
    home = getenv("HOME");
    char *tmp = new char[1024];
    strcpy(tmp, getcwd(cwd, sizeof(cwd)));
    tmp = RepHome(getcwd(cwd, sizeof(cwd)),home,"~");
    cout<<"\nOS Shell Ex2: "<< tmp;
}
 
// This function cuts the last char.
char **Cut_Last(char** parsed, int *length)
{
  char **temp=new char *[(*length)-1];
  int i=0;
  while(i<((*length)-1))
  {
	temp[i]=new char[strlen(parsed[i])];
	strcpy(temp[i],parsed[i]);
	i++;
  }
  return temp;
}

// This function kills a zombie.
int killZombie(char **args,int stat)
{
	pid_t status, pid;
	int temp=stat,tempStat=0;
	status = waitpid(stat, &ExitCode, WNOHANG);
	if(WIFEXITED(stat))
		ExitCode=(WEXITSTATUS(stat));
 if(status!=temp)
{
 pid = fork(); 
 if(pid == 0)
 {
   kill(temp,SIGKILL);
   exit(143);
 }
else if(pid<0) 
 {
    perror("fork error");
 }
 else 
 {
	
	if(waitpid(pid,&ExitCode, 0) == -1)
		perror("wait error");
	else
	{
		if(WIFSIGNALED(ExitCode) != 0)
			ExitCode = 128+WTERMSIG(ExitCode);
		else if(WIFEXITED(ExitCode) != 0)
			ExitCode = WEXITSTATUS(ExitCode);
		else 
			cout << "Not exit normally" << endl;
	}
    
 }
	waitpid(temp, &tempStat, WNOHANG);
 }
 cout << "[" << temp << "] : exited, status = " << ExitCode << "\n";
 return 1;
}

// This funcion checks the string for $ or end of string.
char* CheckVar(char* str)
{
   int i=0, len=strlen(str),CheckExit=0;
   string s;
   string Ostring;
   char *newstr;
   char *newOriginalstr;
   char *var;
   char *temp=new char[strlen(str)];
   strcpy(temp,str);
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
		if(CheckExit==0)
		{
		   var=getenv(newstr);
		   if(var != NULL)
		   	temp=RepHome(temp,newOriginalstr,var);	
		    else cout << "No such variable.\n";	
		}
		else
		{
		    var=new char[1024];
		    sprintf(var, "%d" , ExitCode);
		    temp=RepHome(temp,newOriginalstr,var);	
		}	
		if(str[i]=='$')
			i--;
	   }
	}
	s="";
	Ostring="";
	i++;
   }

   return temp;
}

// This function is for the shell command.
int shellCom(char **args, int length)
{

 pid_t pid;
 int cond=0,status=0;
 if(args[length - 1][0]=='&')
	{
	   cond=1;
	   args=Cut_Last(args,&length);
	   length--;
	}
 pid = fork(); 
 if(pid == 0)
 {
   if(execvp(args[0],args) == -1)
   {
      
	 perror("error in execvp");
      _exit(127);      
   }
   else
	exit(0);
 }
 else if(pid<0) 
 {
    perror("fork error");
 }
 else if(cond==0) 
 {
	if(waitpid(pid,&status, 0) == -1)
		perror("wait error");
	else
	{
		if(WIFSIGNALED(status))
			ExitCode = 128+WTERMSIG(status);
		else if(WIFEXITED(status))
			ExitCode = WEXITSTATUS(status);
		else 
			cout << "Not exit normally" << endl;
	}
 }
 else
 {
    cout<<"\n["<<pid<<"]";
    ExitCode=0;
 }
 return 1;

}

// Function to execute builtin commands.
int ownCmdHandler(char** parsed, int *length)
{

    int NoOfOwnCmds = 6, i, switchOwnArg = 0, succ=0,stat=0,res;
    char** ListOfOwnCmds = new char*[NoOfOwnCmds];
    for (i = 0; i < NoOfOwnCmds; i++) 
	ListOfOwnCmds[i] = new char[10];
    strcpy(ListOfOwnCmds[0], "exit");
    strcpy(ListOfOwnCmds[1], "cd");
    strcpy(ListOfOwnCmds[4], "kill");
    strcpy(ListOfOwnCmds[5], "other");
    char* var;
    for (i = 0; i < NoOfOwnCmds; i++) {
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
 }
   if(switchOwnArg==5 && *length!=1)
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
	
	if(*length==1)
		cout<<"cd: not enough arguments\n";
	else
	{
        	res=chdir(parsed[1]);
		if(res==-1)
	  	{
			perror("cd failed");
	  		ExitCode=1;
	  	}
		else
			ExitCode=0;
	}        
	return 1;
    case 3:
        return 1;
    case 4:

    case 5:
	if(*length==1)
	    cout<<"not enough arguments";	
	else if(killZombie(parsed,stat)==1)
	{
		cout<<"";
	}
	return 1;
    case 6:
	 shellCom(parsed, *length);
	 return 1;
    default:
        break;
    }
    return 0;
}

// function for parsing command words with spaces.
char ** parseSpace(string str, char **parsed, int *length)
{
	string s;
	int i = 0, k = 0, j = 0, count=0,len=str.length();
	while(str[j]==' ')
		j++;
	for(i = j; i<=len ; i++)
	{
	    if(str[i] == ' ' && str[i+1] != ' ' && str[i+1] != '\0')
		count++; 
	    else if(str[i] == '\0')
		count++;
	}
	*length = count;
	parsed=new char *[count+1];
	i=0;
   k = j;
   while(k <= len){  
	if(str[k] == ' ' || str[k]=='\0')
	{
		while(str[k]==' ' && str[k+1]==' ')
			k++;
		s += '\0';
		parsed[i]=new char[s.length()+1];
		strcpy(parsed[i],const_cast<char*>(s.c_str()));
		s="";
		i++;
		k++; 
	}
	    s += str[k];
	k++;
    }
   return parsed;
}
 
// This function takes a string and splits it.
int processString(string str, char **parsed)
{
    int piped = 0, length = 0;
    parsed=parseSpace(str, parsed, &length);
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

    clear();
 
    while (1) {
        // print shell line
        printDir();
        // take input
	inputString=takeInput(inputString);
	if (inputString=="")
            continue;
       // process
            processString(inputString, parsedArgs);
    }
    return 0;
}
