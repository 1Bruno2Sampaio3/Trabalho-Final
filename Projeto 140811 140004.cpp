//Bruno Sampaio      140004
//Gabriel Augusto    140811

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct	info_carro{
	char	sigla;			// L - Livre
	char	local_ret[20];	//local da retirada
	};

struct	info_cliente{
	char	sigla;			// A - Alugado ou R - Reservado
	char	local_ret[20];	//local da retirada
	int		reg_cli;		// nº cliente - buscar no arq. cliente
	int		dia_ret;		// dia - máx=31
	int		mes_ret;		// mes: fev=28 dias
	int		dia_dev;		// mes: abril=jun=set=nov=30 dias
	int		mes_dev;		// os demais = 31 dias
	char	local_dev[20];	//local da devolução
	};

union	estado{
	struct	 info_carro	car;
	struct	 info_cliente	dados[2];
};

typedef	struct	carro{
	int		reg_car;		// gerado automaticamente
	char	modelo[20];
	float	diaria;
	char 	tipo;			// [P]equeno, [M]édio ou [G]rande
	union   estado	status;
} carro;

typedef	struct	cliente{
	int			reg_cli; 		// gerado automaticamente
	char		nome[80];
	char		CPF[15];
	char		sigla;			// [A]lugado ou [R]eservado
	int			reg_car;		// registro do carro - buscar no arq. carro
}cliente;

typedef	struct	vip{
	int			    reg_cli; 		// gerado automaticamente
	char			nome[80];
	char			CPF[15];
	char 			tipo;			// [P]equeno, [M]édio ou [G]rande
}vip;

void aloca_car(carro **p,int tam);
void aloca_cli(cliente **p,int tam);
void aloca_vip(vip **p,int tam);
void cadastro(carro *p,int tam);
void consulta_total(carro *p);
void consulta_parcial(carro *p);
void mostra_cliente(cliente *p);
void mostra_vip(vip *pvip);
void grava_cliente(cliente *p);
void grava_carro(carro *p);
void grava_vip(vip *p);
int verifica_carro();
int verifica_cliente();
int verifica_vip();
void escolha(carro *p,cliente *pcli);
void devolucao(carro *p, cliente *pcli,vip *pvip);
void atualiza_carro(carro *p,int reg);
void atualiza_cliente(cliente *p,int reg);
int verifica_data(int dia,int mes);

main()
{
	cliente *pcli=NULL;
	carro *p=NULL;
	vip *pvip=NULL;
	char op;
	int q,cont=0,qtde,qtdecli;
	aloca_cli(&pcli,cont+1);
	aloca_car(&p,cont+1);
	aloca_vip(&pvip,cont+1);
	do{
		system("cls");
		printf("[1]-Cadastro\n[2]-Reserva/Aluga\n[3]-Mostra\n[4]-Devolucao\n[5]Sair\nopcao: \t");
		scanf("%i",&q);
		fflush(stdin);
		switch(q)
		{
			case 1: qtde = verifica_carro();
					cadastro(p,qtde);
					break;
			case 2: system("cls");
					consulta_parcial(p);
					printf("\nDeseja fazer um cadastro <s/n>: ");
					scanf("%c",&op);
					fflush(stdin);
					if(op=='s')
						escolha(p,pcli);
					break;
			case 3: int n;
					printf("\n[1]-Total\n[2]-Parcial\n[3]-Clientes\n[4]-Historico\nOpcao: ");
					scanf("%i",&n);
					fflush(stdin);
					if(n==1)
						consulta_total(p);
					else if(n==2)
						consulta_parcial(p);
					else if(n==3)
						mostra_cliente(pcli);
					else if(n==4)
						mostra_vip(pvip);
					break;
			case 4: devolucao(p,pcli,pvip);
					break;
		}
	}while(q!=5);
}//main

void aloca_car(carro **p,int tam)
{
	if((*p=(carro*)realloc(*p,tam*sizeof(carro)))==NULL)
		exit(1);
}//aloca_car

void aloca_cli(cliente **p,int tam)
{
	if((*p=(cliente*)realloc(*p,tam*sizeof(cliente)))==NULL)
		exit(1);
}//aloca_cli

void aloca_vip(vip **p,int tam)
{
		if((*p=(vip*)realloc(*p,tam*sizeof(vip)))==NULL)
			exit(1);
}//aloca_vip

void cadastro(carro *p,int tam)
{
	system("cls");
	p->reg_car=tam+1;
	printf("Registro: %i\nModelo: ",p->reg_car);
	gets(p->modelo);
	fflush(stdin);
	strupr(p->modelo);
	printf("Tipo: ");
	scanf("%c",&(p->tipo));
	fflush(stdin);
	strupr(&(p->tipo));
	printf("Valor diario: ");
	scanf("%f",&(p->diaria));
	fflush(stdin);
	/*printf("\nDia da Retirada:\t");
	scanf("%i",&(p->status.dados[0].dia_ret));
	fflush(stdin);
	printf("\nMes da retirada: \t");
	scanf("%i",&(p->status.dados[0].mes_ret));
	fflush(stdin);
	printf("\nDia da Devoilucao: \t");
	scanf("%i",&(p->status.dados[0].dia_dev));
	fflush(stdin);
	printf("\nMes da Devolucao: \t");
	scanf("%i",&(p->status.dados[0].mes_dev));
	fflush(stdin);
	*/p->status.car.sigla='L';
	printf("Status: %c",p->status.car.sigla);
	printf("\nLocal de retirada: ");
	gets(p->status.car.local_ret);
	fflush(stdin);
	strupr(p->status.car.local_ret);
	grava_carro(p);
}//cadastro

void consulta_total(carro *p)
{
	system("cls");
	FILE *fptr=NULL;
	int i;
	long int qtde;
	qtde=verifica_carro();
	if((fptr=fopen("carro.bin","rb"))==NULL)
	  printf("\nErro");
		else
	  {
	  for(i=0;i<qtde;i++)
		{               
	    fseek(fptr,i*sizeof(carro),0);                
	    fread(p, sizeof(carro),1, fptr);
	    printf("Registro: %i", p->reg_car);  
	    printf("\nModelo: %s", p->modelo); 
	    printf("\nTipo: %c",p->tipo);
	    printf("\nValor Diario: %.2f",p->diaria);
	    /*printf("\nDia da Retirada: %i",p->status.dados[0].dia_ret);
	    printf("\nMes da Retirada: %i",p->status.dados[0].mes_ret);
	    printf("\nDia da Devolucao: %i",p->status.dados[0].dia_dev);
	    printf("\nMes da Devolucao: %i",p->status.dados[0].mes_dev);
	    */printf("\nStatus: %c",p->status.car.sigla);
	    printf("\nLocal Retirada: %s\n",p->status.car.local_ret);
	    printf("\n");
		}//for 
	  fclose(fptr);
	  }//else
	  printf("\nTodas opcoes mostradas");
	  printf("\n\n\n");
	  system("pause");
}//consulta_total

void consulta_parcial(carro *p)
{
	system("cls");
	int dia,mes,reg_carro,n,j=0;
	char tipo;
	char local[20];
	do{
		do{
			printf("Insira o mes: ");
			scanf("%i",&mes);
			fflush(stdin);
			printf("Insira o dia: ");
			scanf("%i",&dia);
			fflush(stdin);
			n=verifica_data(dia,mes);
			if(n==-1)
			{
				printf("\nInsira uma data valida\n\n\n");
				system("pause");
				system("cls");
			}//if
		}while(n==-1);
		printf("Tipo do carro (P,M,G): ");
		scanf("%c",&tipo);
		fflush(stdin);
		strupr(&tipo);
		printf("Local para que deseja retirar: ");
		gets(local);
		fflush(stdin);
		strupr(local);
		system("cls");
		FILE *fptr=NULL;
		int i;
		long int qtde;
		qtde=verifica_carro();
		if((fptr=fopen("carro.bin","rb"))==NULL)
		  printf("\nErro");
		else
		{
			for(i=0;i<qtde;i++)
			{
	    		fseek(fptr,i*sizeof(carro),0);                
	    		fread(p, sizeof(carro),1, fptr);
				if(p->tipo == tipo && strcmp(p->status.car.local_ret,local)==0 && p->status.car.sigla!='R')
				{
				    printf("Registro: %i", p->reg_car);  
				    printf("\nModelo: %s", p->modelo); 
				    printf("\nTipo: %c",p->tipo);
				    printf("\nValor Diario: %.2f",p->diaria);
				    /*printf("\nDia da Retirada: %i",p->status.dados[0].dia_ret);
				    printf("\nMes da Retirada: %i",p->status.dados[0].mes_ret);
				    printf("\nDia da Devolucao: %i",p->status.dados[0].dia_dev);
				    printf("\nMes da Devolucao: %i",p->status.dados[0].mes_dev);
				    */printf("\nStatus: %c",p->status.car.sigla);
				    printf("\nLocal Retirada: %s\n",p->status.car.local_ret);
					printf("\n");
					j=-1;
				}//if
			}//for 
	    	fclose(fptr);
	   	 	if(j!=-1)//carro solicitado nao foi encontrado
	   	 	{
	    		consulta_total(p);
				printf("\n\nTODAS AS OPCOES MOSTRADAS POIS DADOS DIGITADOS NAO FORAM ENCONTRADOS\n\n");
				system("pause");
				system("cls");
	   		 }//if
	  	}//else
  	}while(j!=-1);
  	printf("\n\n\n");
  	system("pause");
}//consulta_parcial

int verifica_cliente()
{
	FILE *fp=NULL;
	long int pos=0;
	
	if((fp=fopen("cliente.bin","rb"))==NULL)
	  printf("erro");
	else
	{
	  fseek(fp,0,2);
	  pos=ftell(fp)/sizeof(cliente);
	  fclose(fp);
	}//else
	return pos;
}//verifica_cliente

void grava_cliente(cliente *p)
{
	FILE *fptr=NULL;
	if((fptr=fopen("cliente.bin","ab"))==NULL)
	  printf("\nErro");
	else
	{
	  fwrite(p,sizeof(cliente),1,fptr);
	  printf("\nRegistro salvo com sucesso\n\n");
	}//else
	fclose(fptr);
	system("pause");
}//grava_cliente

void grava_carro(carro *p)
{
	FILE *fptr=NULL;
	if((fptr=fopen("carro.bin","ab"))==NULL)
	  printf("\nErro");
	else
	{
	  fwrite(p,sizeof(carro),1,fptr);
	  printf("\nRegistro salvo com sucesso\n\n");
	}//else
	fclose(fptr);
	system("pause");
}///grava

void grava_vip(vip *p)
{
	FILE *fptr=NULL;
	if((fptr=fopen("vip.bin","ab"))==NULL)
	  printf("\nErro");
	else
	{
	  fwrite(p,sizeof(vip),1,fptr);
	  printf("\nRegistro salvo com sucesso\n\n");
	}//else
	fclose(fptr);
	system("pause");
}

int verifica_carro()
{
	FILE *fp=NULL;
	long int pos=0;
	
	if((fp=fopen("carro.bin","rb"))==NULL)
	  printf("erro na gravacao");
	else
	{
	  fseek(fp,0,2);
	  pos=ftell(fp)/sizeof(carro);
	  fclose(fp);
	}//else
	return pos;
}

void escolha(carro *p,cliente *pcli)
{
	FILE *fp=NULL;
	int i,reg,qtdcli = 1,n = 0;
	char aux[20];
	long int qtdecli,qtdecar;
	qtdecli=verifica_cliente();
	qtdecar=verifica_carro();
	
	do
	{
		printf("\nEscolha o carro digitando o registro: ");
		scanf("%i",&reg);
		fflush(stdin);
	}while(reg < 1 || reg > qtdecar);
	system("cls");
	printf("##  Cadastro de Usuario  ##\n\n");
	do
	{
		n=0;
		printf("Digite o CPF: ");
		gets(aux);//(pcli+tamcli)->CPF
		fflush(stdin);
		
		if((fp=fopen("cliente.bin","rb"))==NULL)
		  printf("\nErro");
		else
		{
		  for(i=0;i<qtdecli;i++)
	      {               
		    fseek(fp,i*sizeof(cliente),0);                
		    fread(pcli, sizeof(cliente),1, fp);
		    if(strcmp(pcli->CPF,aux)==0)
			{
				printf("\nCPF ja cadastrado\n");
				i=qtdecli;
				n = 1;
			}//if
		  }//for
		}//else
	}while (n == 1);
	strcpy(pcli->CPF,aux); 
	fclose(fp);
	
	pcli->reg_cli = qtdecli+1;
	printf("Nome: ");
	gets(pcli->nome);
	fflush(stdin);
	strupr(pcli->nome);
	pcli->reg_car = reg;
	if((fp=fopen("carro.bin","rb"))==NULL)
	  printf("\nErro");
	else
	{
    		fseek(fp,(reg-1)*sizeof(carro),0);                
    		fread(p, sizeof(carro),1, fp);
			if(p->status.car.sigla == 'L')
			{
			 	p->status.car.sigla = 'A';
			 	pcli->sigla = 'A';
				pcli->reg_cli = qtdecli+1;   
			}//if
			
			else if(p->status.car.sigla == 'A')
			{
			 	p->status.car.sigla = 'R';
			 	pcli->sigla = 'R';
				pcli->reg_cli = qtdecli+1;   
			}//else if	
			
			 /*else
			{
				n=-1;
			 	printf("\n\n\n\nCarro ja Reservado\n\n\n\n");
			 	printf("Registro: %i", p->reg_car);  
			    printf("\nModelo: %s", p->modelo); 
			    printf("\nTipo: %c",p->tipo);
			    printf("\nValor Diario: %.2f",p->diaria);
			    printf("\nStatus: %c",p->status.car.sigla);
			    printf("\nLocal Retirada: %s\n",p->status.car.local_ret);
				printf("\n");
				system("pause");
			}//else
			*/
    fclose(fp);
  	}//else	
  	//if(n!=-1)
  		grava_cliente(pcli);
  	atualiza_carro(p,reg);
	mostra_cliente(pcli);
	//qtdcli++;
}//escolha

void mostra_cliente(cliente *p)
{
	system("cls");
	FILE *fptr=NULL;
	int i;
	long int qtde;
	qtde=verifica_cliente();
	if((fptr=fopen("cliente.bin","rb"))==NULL)
	  printf("\nErro");
		else
	  {
	  for(i=0;i<qtde;i++)
		{               
	    fseek(fptr,i*sizeof(cliente),0);                
	    fread(p, sizeof(cliente),1, fptr);
	    printf("Registro: %i",p->reg_cli);  
	    printf("\nNome: %s",p->nome); 
	    printf("\nCPF: %s",p->CPF);
	    printf("\nRegistro do Carro: %i",p->reg_car);
	    printf("\nSigla: %c\n",p->sigla);
	    printf("\n");
		}//for 
	  fclose(fptr);
	  }//else
	  printf("\n");
	  system("pause");
}//mostracliente

void devolucao(carro *p, cliente *pcli,vip *pvip)
{
	FILE *fp=NULL;
	FILE *fptr=NULL;
	int i,j,k,dia,mes,res,n;
	float multa;
	char aux[20],op;
	long int qtdecli,qtdecar,qtdevip;
	qtdecli=verifica_cliente();
	qtdecar=verifica_carro();
	qtdevip=verifica_vip();
	do
	{
		printf("\nCPF do usuario que ira devolver: ");
		gets(aux);
		fflush(stdin);
		
		if((fp=fopen("cliente.bin","rb"))==NULL)
			  printf("\nErro");
		else
		{ 
			for(i=0;i<qtdecli;i++)
		    {               
				fseek(fp,i*sizeof(cliente),0);                
			    fread(pcli, sizeof(cliente),1, fp);
				if(strcmp(pcli->CPF,aux)==0 && pcli->sigla=='A')
				{
					if((fptr=fopen("carro.bin","rb"))==NULL)
				  	printf("\n\nerro ao abrir o arquivo\n\n\n");
					else
					{
						for(j=0;j<qtdecar;j++)
					    {               
							fseek(fptr,j*sizeof(carro),0);                
							fread(p, sizeof(carro),1, fptr);
							if(p->reg_car==pcli->reg_car)
							{
								k=-1;
								j=qtdecar;
								i=qtdecli;
								do
								{
									printf("Insira o mes: ");
									scanf("%i",&mes);
									fflush(stdin);
									printf("\nInsira o dia: ");
									scanf("%i",&dia);
									fflush(stdin);
									n=verifica_data(dia,mes);
									if(n==-1)
									{
										printf("\nInsira uma data valida\n\n\n");
										system("pause");
										system("cls");
									}
								}while(n==-1);
																
								pvip->reg_cli=qtdevip+1;
								strcpy(pvip->nome,pcli->nome);
								strcpy(pvip->CPF,pcli->CPF);
								pvip->tipo=p->tipo;
								grava_vip(pvip);
									
								printf("\nDeseja mostras historico <s/n>: ");
								scanf("%c",&op);
								fflush(stdin);
								if(op=='s'|| op=='S')
									mostra_vip(pvip);	
									
									/*
									pcli->reg_cli = NULL;
									pcli->nome = NULL;
									pcli->CPF = NULL;
									pcli->reg_car = NULL;
									p->status.car.sigla = NULL;
									printf("Registro: %i",pcli->reg_cli);  //mostra o arquivo vazio
								    printf("\nNome: %s",pcli->nome); 
								    printf("\nCPF: %s",pcli->CPF);
								    printf("\nRegistro do Carro: %i",pcli->reg_car);
								    printf("\nSigla: %c\n",p->status.car.sigla);
								    printf("\n");
								    */
									    
									 //res e = qtd de dias atrasado.
									//res = dia - p->status.dados[0].dia_dev;   
									
									//res=-5;
									if(res <= 0)//dará menor que zero se o usuário devolver antes da data e iguala zero se ele devolver no dia previsto em ambos os casos não será cobrado multa
									{
										printf("\nAluguel sem cobranca de multa por atraso\n");
										if(p->status.car.sigla=='A')
										{
											pcli->sigla='L';//cliente que estava akugado passa livre
											pcli->reg_car=0;//coloka 0 no registro do carro
											atualiza_cliente(pcli,pcli->reg_cli);
											p->status.car.sigla='L';	
											atualiza_carro(p,p->reg_car);
										}
									
										if(p->status.car.sigla == 'R')
									    {
									    	pcli->sigla='L';//cliente que estava akugado passa livre
									    	pcli->reg_car=0;//coloka 0 no registro do carro
											atualiza_cliente(pcli,pcli->reg_cli);
									    	for(i=0;i<qtdecli;i++)// busca clinte com reserve deste carro
										    {               
												fseek(fp,i*sizeof(cliente),0);                
											    fread(pcli, sizeof(cliente),1, fp);
												if(p->reg_car==pcli->reg_car && pcli->sigla=='R')//encontrou cliente com reserva do carro
												{
													pcli->sigla='A';// passando de reservado para alugado
													atualiza_cliente(pcli,pcli->reg_cli);
												}						    	
											//p->status.dados[0].dia_ret = dia;//alteracao de datas de acordo com a devolucao feita pelo cliente anterior
									    	//p->status.dados[0].mes_ret = mes;
									    	p->status.car.sigla = 'A';// de Reservado passa para alugado.	
											atualiza_carro(p,p->reg_car);
											}//if
											system("pause");
										}//if
									}//if
									
									if(res > 0)
									{
										multa = (p->diaria * res * 2);//será cobrado 100% por dia de atraso.
										printf("\nDevolucao com Multa\n\n\n\tTotal a pagar: R$ %.2f\n",multa);
										if(p->status.car.sigla=='A')
										{
											pcli->sigla='L';//cliente que estava akugado passa livre
											pcli->reg_car=0;//coloka 0 no registro do carro
											atualiza_cliente(pcli,pcli->reg_cli);
											p->status.car.sigla='L';		
											atualiza_carro(p,p->reg_car);
										}
										if(p->status.car.sigla == 'R')
										{
											pcli->sigla='L';//cliente que estava akugado passa livre
											pcli->reg_car=0;//coloka null no registro do carro
											atualiza_cliente(pcli,pcli->reg_cli);
											for(i=0;i<qtdecli;i++)// busca clinte com reserve deste carro
										    {               
												fseek(fp,i*sizeof(cliente),0);                
											    fread(pcli, sizeof(cliente),1, fp);
												if(p->reg_car==pcli->reg_car && pcli->sigla=='R')
												{
													pcli->sigla='A';// passando de reservado para alugado
													atualiza_cliente(pcli,pcli->reg_cli);
												}	
											}
										    //p->status.dados[0].dia_ret = dia;//alteracao de datas de acordo com a devolucao feita pelo cliente anterior
										    //p->status.dados[0].mes_ret = mes;
										   	p->status.car.sigla = 'A';// de Reservado passa para alugado.	
											atualiza_carro(p,p->reg_car);
										}//if
										system("pause");
									}//if
				  			}//if
						}//for
						fclose(fptr);
					}//else
				}//if
			}//for
			fclose(fp);
		}//else
		if(k!=-1) // k!=-1 nao possui carro alugado
		{
			printf("\nCPF nao possui carro alugado\n\n");
			system("pause");
		}
	}while(k!=-1);
}//devolucao

void mostra_vip(vip *pvip)
{
	FILE *fp = NULL;
	char op;
	long int qtde;
	int i;
	qtde=verifica_vip();
	system("cls");
	if((fp=fopen("vip.bin","rb")) == NULL)
		printf("\nErro ao Abrir o Arquivo\n\n");
			
	else
	{
		for(i=0;i<qtde;i++)
		{
			fseek(fp,i*sizeof(vip),0);
			fread(pvip,sizeof(vip),1,fp);
			printf("Registro: %i",pvip->reg_cli);
			printf("\nNome: %s",pvip->nome); 
			printf("\nCPF: %s",pvip->CPF);
			printf("\nTIPO: %c\n",pvip->tipo);
			printf("\n");
		}
		fclose(fp);
		system("pause");
	}//else
}//mostra_vip

void atualiza_carro(carro *p,int reg)
{
	FILE *fp=NULL;
	
	if((fp=fopen("carro.bin","rb+"))==NULL)
		printf("erro");
	else
	{
		fseek(fp,(reg-1)*sizeof(carro),0);
		fwrite(p,sizeof(carro),1,fp);
	}
	fclose(fp);
}//atualiza_carro

void atualiza_cliente(cliente *p,int reg)
{
	FILE *fp=NULL;
	
	if((fp=fopen("cliente.bin","rb+"))==NULL)
		printf("erro");
	else
	{
		fseek(fp,(reg-1)*sizeof(cliente),0);
		fwrite(p,sizeof(cliente),1,fp);
	}
	fclose(fp);
}//atualiza_cliente

int verifica_vip()
{
	FILE *fp=NULL;
	long int pos=0;
	
	if((fp=fopen("vip.bin","rb"))==NULL)
	  printf("erro");
	else
	{
	  fseek(fp,0,2);
	  pos=ftell(fp)/sizeof(vip);
	  fclose(fp);
	}//else
	return pos;
}

int verifica_data(int dia,int mes)
{
	if(mes<1 || mes >12)
	{
		printf("\nMes invalido");
		return-1;
	}
	  if(mes==2)
		if(dia>0 && dia<29)
			return 1;
	   if(mes==4 || mes==6 || mes==9 || mes==11)
		 if(dia>0 && dia<31)
			return 1;
	 if(mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10 || mes==12)
		if(dia>0 && dia<32)
			return 1;
		
	printf("\nDia invalido");
	return -1;
}//verifica_data
