# Trabalho Final de Programação Paralela
Nome: Leonardo Marcuzzo\
Matrícula: 201130409

## Descriçao do Problema

Ao usar o gnuplot para gerar gráficos grandes ou densos, o plot pode demorar para ser feito, enquanto o uso de CPU não é muito alto(em CPUs Multicore). Isto acontece devido ao programa utilizar metodos sequenciais para a leitura do arquivo e plotagem dos dados.

O arquivo de entrada a ser usado nestes testes com nome **an4** contém 700 mil linhas e 8 colunas, contendo valores do tipo **float** com 2 casas decimais.

Em relação a tempo de execução, o código a seguir plota 6 linhas de dados, cada uma com 100 mil pontos:
```
set term svg
set output "test"
a="an4"; plot [350000:450000] a u 5 w l, a u 3 w l, a u 7 w l, a u 2 w l, a u ($8/25) w l, a u ($9/40) w l
```

O tempo de execução deste código é de

>real	0m7.793s\
>user	0m7.262s\
>sys	0m0.538s

## Objetivo
O objetivo deste trabalho é buscar oportunidades de paralelizaçao no código do gnuplot, e tentar paralelizar estas oportunidades.

## Desenvolvimento
Primeiro, o código fonte do gnuplot foi baixado de http://sourceforge.net/projects/gnuplot/files/

### Estrutura do programa

A função principal do gnuplot é basicamente um parser que em primeiro lugar identifica se o usuário quer entrar em modo iterativo ou está passando um arquivo como parâmetro.

Independente da escolha, o gnuplot passa a receber chamadas de métodos, como definir qual será o método de saida, nome do arquivo de saída(se ouver), labels, tipo de plot e etc.

No caso apresentado, os comandos estão definidos em um arquivo chamado **plot.plg** que apenas seta o tipo de saída(SVG), o nome do arquivo, e chama o arquivo **an4** com os parâmetros do plot.

Utilizando o programa **gprof**, é possível ver de que maneira o programa faz a chamada das funções.

!(prof.png)

A função `get_data()` contida em src/plot2d.c (estamos desconsiderando chamadas de plot em 3D) é chamada 6 vezes, uma para cada linha a ser plotada.

```
static int
get_data(struct curve_points *current_plot)
{
    int i /* num. points ! */ , j;
    int ngood;
    ...
    
    while ((j = df_readline(v, max_cols)) != DF_EOF) {

	...
    
    if (current_plot->plot_style == PARALLELPLOT && j > 0) {
	    int iaxis;
	    if (j != current_plot->n_par_axes)
		fprintf(stderr,"Expecting %d input columns, got %d\n",
			current_plot->n_par_axes, j);
	    store2d_point(current_plot, i, 1.0, v[0], 
				0.5, (double)(current_plot->n_par_axes)+0.5,
				v[0], v[0], 0.0);
                ...
       }
    }
}

```

Devido ao tamanho da função, a maior parte dela foi omitida do texto. Pelo que se pode inferir do código, a função `df_readline()` é chamada para cada linha do arquivo daonde os dados são extraidos, e ela chama funções auxiliares `df_readascii()` por ser um arquivo de texto (no caso de um arquivo binário, a função a ser chamada seria `df_readbinary()`) que por sua vez chama `df_tokenize()`, que é a função que ocupa a maior parte do processamento. Estas funções estão contidas em src/datafile.c

Mais abaixo mas ainda dentro do laço de onde a função `df_readline()` é chamada, a função `store2d_point()` salva os pontos em memória para plotar o gráfico.

Após a função `get_data()` ter sido finalizada, a função `plot_lines()` é chamada da função `main()` para cada uma das linhas a serem plotadas, mas como o tempo dela é negligível em relação as outras, não tem por que paralelizar ela. A função `SVG_Vector()` é chamada pela `plot_lines()` para fazer o mapeamento dos pontos e gravar a saída.

### Oportunidades de Paralelização

#### df_tokenise()

Dentre as funções comentadas na seção anterior, as que mais se destacam são `df_tokenise()`, pelo seu tempo gasto com processamento, e a função `df_readascii()`.


```
static int
df_tokenise(char *s)
{
    TBOOLEAN in_string;
    int i;

    if (s[strlen(s)-1] == '\n' || s[strlen(s)-1] == '\r')
	s[strlen(s)-1] = '\0';

    for (i = 0; i<MAXDATACOLS; i++)
	df_tokens[i] = NULL;

    df_no_cols = 0;
	...
	
    if (*s == '"') {
	    /* treat contents of a quoted string as single column */
	    in_string = !in_string;
	    df_column[df_no_cols].good = DF_STRINGDATA;
	} else if (check_missing(s)) {
	    df_column[df_no_cols].good = DF_MISSING;
	    df_column[df_no_cols].datum = not_a_number();
	    df_column[df_no_cols].position = NULL;
	} else {
	    int used;
	    int count;
	    int dfncp1 = df_no_cols + 1;

	    /* optimizations by Corey Satten, corey@cac.washington.edu */
	    /* only scanf the field if it is mentioned in one of the using specs */
	    if ((fast_columns == 0)
		|| (df_no_use_specs == 0)
		|| ((df_no_use_specs > 0)
		    && (use_spec[0].column == dfncp1
			|| (df_no_use_specs > 1
			    && (use_spec[1].column == dfncp1
				|| (df_no_use_specs > 2
				    && (use_spec[2].column == dfncp1
				|| (df_no_use_specs > 3
				    && (use_spec[3].column == dfncp1
				        || (df_no_use_specs > 4
				            && (use_spec[4].column == dfncp1
				                || df_no_use_specs > 5)
				            )
				        )
				    )
				)
				    )
				)
			    )
			)
		    )
		) 
```
Basicamente, a função procura na linha valores válidos e retorna um vetor de tokens. Isto faz com que a função gaste bastante tempo procurando e associando os dados da linha, mas uma linha é independente da outra, e por isso ela poderia ser executada em paralelo com as **n** linhas seguintes.

O problema é que essa função é chamada com dados passados pela função `df_readascii()`, e portanto para paralelizar a `df_tokenise()`esta é necessário paralelizar a `df_readascii()`.

#### df_readascii()

```
int
df_readascii(double v[], int max)
{
    char *s;
    int return_value = DF_GOOD;

    assert(max <= MAXDATACOLS);

    /* catch attempt to read past EOF on mixed-input */
    if (df_eof)
	return DF_EOF;
	...
    
    	if (df_format) {
	    /*{{{  do a sscanf */
	    int i;

	    /* check we have room for at least 7 columns */
	    if (df_max_cols < 7)
		expand_df_column(7);

	    df_no_cols = sscanf(s, df_format,
				&df_column[0].datum,
				&df_column[1].datum,
				&df_column[2].datum,
				&df_column[3].datum,
				&df_column[4].datum,
				&df_column[5].datum,
				&df_column[6].datum);

	    if (df_no_cols == EOF) {
		df_eof = 1;
		return DF_EOF;  /* tell client */
	    }
	    for (i = 0; i < df_no_cols; ++i) {  /* may be zero */
		df_column[i].good = DF_GOOD;
		df_column[i].position = NULL;   /* cant get a time */
	    }
	    /*}}} */
	} else
	    df_tokenise(s);
	...     

```
OBS: A função `df_readline()` simplesmente repassa os parâmetros de `get_data()` para esta função ou para `df_readbinary()`, logo ela não tem muito código e não é importante para causar problemas ou otimizar o código.

A função `df_readascii()` se destaca pois além de chamar as outras, ela própria é chamada várias vezes. Vendo novamente o relatório do **gprof**, a função `get_data()` é chamada 6 vezes pois estamos plotando 6 linhas diferentes. Mas a função `df_readascii()` e suas filhas (com exceção de `check_missing()` que apenas verifica a entrada procurando linhas ou espaços em branco) são chamadas 4.2 milhões de vezes, sendo que estamos plotando um gráfico com 100 mil pontos, porque ela não é chamada 600 mil vezes(como pode ser visto na `SVG_Vector()`)?

Por algum motivo(qual motivo??) a função é chamada para todas as 700 mil linhas, mesmo que apenas 100 mil sejam usadas, e é chamada 6 vezes (linhas plotadas) também, totalizando as 4.2 milhões de chamadas.

Esta função retorna o vetor de tokens completo da linha, que será usado na `plot_lines()`.

## Tentativas de paralelização

Em primeiro lugar, dentro da função `get_data()` foi tentada a paralelização do laço:


```
get_data(){

	...

	int conta = 0;
    //Assumindo arquivo com 700 mil linhas
    #pragma omp parallel for default(shared) private(j)
    for(conta = 0 ; conta <= 2100000 ; conta++){

    //Display n da thread
    printf("%d\n",omp_get_thread_num());

    j = df_readline(v, max_cols);	

	if (i >= current_plot->p_max) {
	    /* overflow about to occur. Extend size of points[]
	     * array. Double the size, and add 1000 points, to avoid
	     * needlessly small steps. */
	    cp_extend(current_plot, i + i + 1000);
	}
	
    ...

```
Apesar do programa funcionar sem problemas de maneira sequencial, com duas ou mais threads acontece um erro de *segmentation fault*. Embora eu não tenha certeza, acredito que o problema seja relacionado a chamada da função ``store2d_point()`` mais abaixo do código, devido ao fato da função alocar dinamicamente a memória dos ponteiros.

## Próximos Passos

Embora a oportunidade de paralelização achada pareça ser boa, o tamanho da função `get_data()` dificulta o processo por causa da quantidade de variáveis globais sendo usadas. Para se fazer uma paralelização efetiva, deve-se verificar quais variáveis estão diretamentes ligadas a chamada da função `df_readline()` e quais outras variáveis de teste podem ser selecionadas como privadas para que cada thread possa armazenar suas informações, de modo que cada linha pode ser interpretada de maneira diferente e sem impor problemas a outras linhas.