//----Author: Kshitij Kansal
//----Roll No: 201101031
#include<stdio.h> 
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
char path[1000];
int a=0;
struct process
{
	int flag;
	pid_t pid;
	char *name;
};
struct process N[100];
void print(void)
{
	char* host=(char*)malloc(sizeof(char)*100);
	char* mac=(char*)malloc(sizeof(char)*100);
	char* host1=(char*)malloc(sizeof(char)*100);
	gethostname(mac,100);
	FILE *fp = popen("whoami","r");
	fgets(host,100,fp);
	pclose(fp);
	int i=0;
	while(host[i]!='\n')
	{
		host1[i]=host[i];
		i++;
	}
	char path1[1000],curdir[1000];
	getcwd(path1,1000);
	if(strcmp(path,path1)==0)
		strcpy(curdir,"~");
	else if(strcmp(path,path1)<0)
	{
		int ii,jj,ka=0;
		char new[1000];
		strcpy(curdir,"~");
		ii=strlen(path1);
		for(jj=strlen(path);jj<ii;jj++)
		{
			new[ka]=path1[jj];
			ka++;
		}
		strcat(curdir,new);
	}
	else
	{
		getcwd(curdir,1000);
	}
	printf("<%s@%s:%s>",host1,mac,curdir);
	fflush(stdout);
}
void signal_handler(int signo)
{
	printf("\n");
	print();
}
void child_handler(int signo)
{
	pid_t pid=waitpid(WAIT_ANY,NULL,WNOHANG);
	int i;
	for(i=0;i<a;i++)
	{
		if(N[i].pid==pid)
		{
			N[i].flag=0;
			printf("\n%s %d exited normally\n",N[i].name,N[i].pid);
			print();
		}
	}
}
void redirection(int indicator[],int f[],char *argument[],int k)
{
	int i,j=0;
	printf("k is %d\n",k);
	while(j!=k)
	{
		if (indicator[j]==1)
		{
			i=0;
			/*while(strcmp(input_str[i],"<")!=0)
			  {
			  cmd[i]=strdup(input_str[i]);
			  i++;
			  }
			  cmd[i]=NULL;*/
			f[j]=open(argument[j], O_RDONLY);
			dup2(f[j],0);
			close(f[j]);

		}
		else if(indicator[j]==2)
		{
			/*printf("inside indicator 2\n");
			  i=0;
			  while(strcmp(input_str[i],">")!=0)
			  {
			  cmd[i]=strdup(input_str[i]);
			  i++;
			  }
			  cmd[i]=NULL;*/
			f[j]=open(argument[j],O_WRONLY | O_CREAT,0666);
			dup2(f[j],1);
			close(f[j]);
		}
		j++;
	}
	//	return *cmd;
}

	int main(void)
	{
		getcwd(path,1000);
		int l=0,i=0;
		int kshitij=getpid();
		char **logs;
		logs=(char**)malloc(sizeof(char*)*1000);
		for(i=0;i<1000;i++)
			logs[i]=(char*)malloc(sizeof(char)*1000);
	while(1)
	{
		signal(SIGINT,signal_handler);
		signal(SIGTSTP,signal_handler);
		signal(SIGCHLD,child_handler);
		pid_t pid;
		int k=0,p=0;
		int status,in=0,indicator[100],f[10],chk=0;
		//printf("indicato_initial = %d\n",indicator);
		char* input=(char*)malloc(sizeof(char)*10);
		print();
		gets(input);
		if(strlen(input)==0)
			continue;
		char delims[] = " \t";
		char *result = NULL;
		char *input_str[100];
		char input1[100];
		strcpy(input1,input);
		result = strtok(input ,delims);
//			printf("seg fault is coming here\n");
		while( result != NULL ) {
		//	printf("inside\n");
			//printf( "result is \"%s\"\n", result );
			input_str[in]=strdup(result);
			if(strcmp(input_str[in],"<")==0)
			{
				indicator[k++]=1;
				chk=1;
			}
			if(strcmp(input_str[in],">")==0)
			{
				indicator[k++]=2;
				chk=1;
			}
			if(strcmp(input_str[in],"|")==0)
			{
				p++;
			}
			//	else if(strcmp(result,">")==0)
			//		indicator=1;
			in++;
			result = strtok(NULL, delims);
		}
		logs[l]=strdup(input);
		N[a].name=strdup(input_str[0]);
		N[a].flag=1;
		l++;
		if(strcmp(input_str[0],"quit")==0)
			exit(0);
//		printf("chk = %d\n",chk);
/*		if(chk==1)
		{
		if(strcmp(input_str[in-2],"<")==0)
			indicator=1;
		else if(strcmp(input_str[in-2],">")==0)
			indicator=2;
		}*/
//		printf("I am here0\n");
		if(input[0]=='!'&&input[1]=='h'&&input[4]=='t')
		{
			in=0;
			int pp;
			pp=input[5]-'0';
			strcpy(input,logs[pp-1]);
			result = strtok( input , delims );
			while(result != NULL ) 
			{
				input_str[in]=strdup(result);
				in++;
				result = strtok( NULL, delims );
			}

		}
		if (strcmp(input_str[0],"cd")==0)
		{
			chdir(input_str[1]);
			continue;
		}
		
//		printf(" I am here\n");
		pid=fork();
		if(pid<0)
		{
			printf("child not created");
		}
		else if(pid == 0)
		{
			if(p!=0)
			{
				int in1=0,fd[100][2];
				char *str_pid[100];
				pid_t pid1[100];
				//	delims[]="|";
			//	printf("input is %s\n",input1);
				char *result = strtok(input1 ,"|");
				while( result != NULL ) {
					str_pid[in1]=strdup(result);
					in1++;
					result = strtok(NULL,"|");
				}
				for(i=0;i<p+1;i++)
				{
				char *cmd[100];
				char *result = strtok(str_pid[i]," \t");
//				printf("str_pid %s\n",str_pid[i]);
				int count=0,v;
				while( result != NULL ) {
					cmd[count]=strdup(result);
					count++;
					result = strtok(NULL," \t");
				}
				cmd[count]=NULL;
				int var1,chk1=0;
				for(var1=0;var1<count;var1++)
				{
					if(strcmp(cmd[var1],"<")==0 || strcmp(cmd[var1],">")==0)
						chk1=1;
				}
//				for(v=0;v<count;v++)
//				{
//					printf("what %s\n",cmd[v]);
//				}

					pipe(fd[i]);
					pid1[i]=fork();
						if(chk1==1)
						{
							int var,lo=0,k=0,f[100];
							char *cmd1[100],*argument[100];
							for(var=0;var<count;var++)
							{

								if(strcmp(cmd[var],"<")==0)
								{
									indicator[k]=1;
									argument[k++]=strdup(cmd[var+1]);
									cmd[var]=NULL;
								}
								else if(strcmp(cmd[var],">")==0)
								{
									indicator[k]=2;
									argument[k++]=strdup(cmd[var+1]);
									cmd[var]=NULL;
								}
							}
	//						for(var=0;var<count;var++)
	//						{
	//							if(strcmp(cmd[var],"<")==0 || strcmp(cmd[var],">")==0)
	//								break;
	//							cmd1[lo++]=strdup(cmd[var]);
	//						}
	//						cmd1[lo]=NULL;
			//				printf("cmd1 0 + %s\n",cmd[0]);
			//				printf("cmd1 1 + %s\n",cmd[1]);
							redirection(indicator,f,argument,k);
							//execvp(*cmd,cmd);
						}
					if(pid1[i]==-1)
						printf("child NOT created\n");
					else if(pid1[i]==0)
					{
				//		printf("mayank\n");

						if(i==0)
						{
							close(fd[i][0]);
					//		dup2(fd[i][1],1);
					//close(fd[i][1]);
						}
						else
						{
							dup2(fd[i-1][0],0);
						}
						if(i==p)
						{
							close(fd[i][1]);
						}
						else
						{
							dup2(fd[i][1],1);
						}
						int j;
						for(j=0;j<=i;j++)
						{
							close(fd[j][0]);
							close(fd[j][1]);
						}
						//		printf("mayank1\n");
						//	if(chk1!=1)
						if(strcmp(input_str[0],"pid")==0)
						{
							if(in >1)
							{
								if(strcmp(input_str[1],"all")==0)
								{
									int r;
									printf("List of all processes spawned from this shell\n");
									for(r=0;r<a;r++)
									{
										printf("command name: %s process id:",N[r].name);
										printf("%d\n",N[r].pid);
									}
								}
								else if(strcmp(input_str[1],"current")==0)
								{
									int r;
									printf("List of currently running processes spawned from this shell\n");
									for(r=0;r<a;r++)
									{
										if(N[r].flag==1)
										{
											printf("command name: %s process id:",N[r].name);
											printf("%d\n",N[r].pid);
										}

									}
								}
							else
							{
								printf("command name:./a.out process id: ");
								printf("%d\n",kshitij);
							}
							}
							else
							{
								printf("command name:./a.out process id: ");
								printf("%d\n",kshitij);
							}
							continue;
						}
						if(strcmp(input,"hist")==0)
						{
							int kk;
							for(kk=0;kk<l;kk++)
								printf("%s\n",logs[kk]);
							continue;
						}
						else if(strlen(input)>4)
						{
							int kk1;
							char input1[100];
							for(kk1=0;kk1<4;kk1++)
							{
								input1[kk1]=input[kk1];
							}
							input1[4]='\0';
							if(strcmp(input1,"hist")==0)
							{
								int nos;
								nos=input[4]-'0';
								kk1=l;
								if(nos<kk1)
								{
									for(i=kk1-nos;i<kk1;i++)
										printf("%s\n",logs[i]);
								}
								else
								{
									for(i=0;i<kk1;i++)
										printf("%s\n",logs[i]);
								}
								continue;
							}
						}
						else
							if(execvp(*cmd,cmd)==-1)
								printf("NO command");

					}
					else
					{
						if(i!=p)
							continue;
						else
						{
							int j;
							for(j=0;j<1;j++)
							{
								close(fd[j][0]);
								close(fd[j][0]);
							}
							for(j=0;j<1;j++)
								waitpid(pid1[j],NULL,0);
						}
					}
				}
			}
			else
			{
//				printf("I am inside child\n");
				k=0;
				char **cmd=(char**)malloc(sizeof(char*)*100);
				//	printf("indicator in child = %d\n",indicator);
				char *argument[100];
						if(strcmp(input_str[0],"pid")==0)
						{
							if(in >1)
							{
								if(strcmp(input_str[1],"all")==0)
								{
									int r;
									printf("List of all processes spawned from this shell\n");
									for(r=0;r<a;r++)
									{
										printf("command name: %s process id:",N[r].name);
										printf("%d\n",N[r].pid);
									}
								}
								else if(strcmp(input_str[1],"current")==0)
								{
									int r;
									printf("List of currently running processes spawned from this shell\n");
									for(r=0;r<a;r++)
									{
										if(N[r].flag==1)
										{
											printf("command name: %s process id:",N[r].name);
											printf("%d\n",N[r].pid);
										}

									}
								}
							else
							{
								printf("command name:./a.out process id: ");
								printf("%d\n",kshitij);
							}
							}
							else
							{
								printf("command name:./a.out process id: ");
								printf("%d\n",kshitij);
							}
						//	continue;
						}
				if(chk!=0)
				{
					for(i=0;i<in;i++)
					{
						if(strcmp(input_str[i],"<")==0 || strcmp(input_str[i],">")==0)
							break;
						cmd[i]=strdup(input_str[i]);
					}
					cmd[i]=NULL;
					for(i=0;i<in;i++)
					{
						if(strcmp(input_str[i],"<")==0 || strcmp(input_str[i],">")==0)
						{
							argument[k++]=strdup(input_str[i+1]);
						}
					}
			//		printf("chk inside child = %d\n",chk);
					redirection(indicator,f,argument,k);
				}
				else 
				{
					for(i=0;i<in;i++)
					{
						cmd[i]=strdup(input_str[i]);
					}
					cmd[i]=NULL;
				}
				if(execvp(*cmd, cmd)==-1)
					printf("ERROR: NO command");			// Replaces the address space of this program with the specified program
				_exit(0);
			}

				_exit(0);
		}
		else{
			N[a].pid=pid;
			a++;
			if(in>1)
			{
				if(strcmp(input_str[in-1],"&")!=0)
				{
					signal(SIGCHLD,NULL);
					wait(NULL);
					N[a-1].flag=0;
					//signal(SIGCHLD,child_handler);

				}
				else if(strcmp(input_str[in-1],"&")==0)
				{
					printf("Command %s pid %d\n",N[a-1].name,N[a-1].pid);
				}
			}
			else
			{
				N[a-1].flag=0;
				wait(&status);
			}
	//		printf("inside big daddy\n");
		//	printf("indicator = %d\n",indicator);
		}
	}
	return 0;
}

