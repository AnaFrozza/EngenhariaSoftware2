
# Funcao que faz a contagem
def count(conteudo):
	quant_func = 0
	quant_laco = 0
	quant_com = 0
	
	for linha in conteudo:
		valor = linha.split()
		if(len(valor) != 0):
			#Conta funcao
			if(valor[0] == 'def'):
				quant_func += 1
			#conta laco
			if(valor[0] == 'for'):
				quant_laco += 1
			#conta comentarios
			if(valor[0] == '#'):
				quant_com += 1
			
	print("Tamanho total do programa: %d linhas" % len(conteudo))
	print("Quantidade de funcoes: %d "% quant_func)
	print("Quantidade de lacos: %d "% quant_laco)
	print("Quantidade de comentarios: %d "% quant_com)


if __name__ == '__main__':
	from sys import argv

	f = open('teste.py', 'r')
	conteudo = f.readlines()
	count(conteudo)

	f.close()
	print("\nConcluido !")
	