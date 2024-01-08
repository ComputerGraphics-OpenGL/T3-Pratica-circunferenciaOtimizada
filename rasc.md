# Conversão Matricial de circunferências

* A equação de uma circunferência com centro na origem e raio R, em coordenadas cartesianas, é dada por:
x^2 + y^2 = R^2

Circunferências não centradas na origem podem ser transladadas para a origem, e os pixels reais podem ser traçados aplicando-se um offset aos pixels gerados pelo algoritmo de conversão matricial.
Equação explícita da circunferência, y = f(x):
y = +-raizQuadrada(((R^2) - (x^2)))

Um arco de 1/4 de circunferência, obtido variando-se x em incrementos unitários, e calculando e arredonadndo y.

Para desenhar 1/4 de cincunferência (os outros 3/4 são desenhados por simetria), poderíamos criar x de 0 a R, em incrementos de uma unidade, calculando +y a cada passo através da equação acima. Essa estratégia funciona, mas é ineficiente porque requer operações de multiplicação e raiz quadrada. Além disso, haverá grandes gaps nas regiões onde a tangente à circunferência é infinita (valores de x próximos a R). Uma maneira de resolver o problema dos gaps é "plotas" x = R * cos Teta e y = R * sen Teta, variando de 0 a 90 graus, mas essa solução também é ineficiente, pois precisa de funções caras (sen e cos).

# Simetria de ordem 8
Note que o traçado de uma cincunferência pode tirar proveito de sua simetria. Considere uma cinrcunferência centrada na origem. Se o ponto (x, y) pertence à circunferencia. Consequentemente, basta computar um arco de cincunferência de 45° para obter a circunferencia toda. Para circunferencia com centro na origem, os oito pontos simétricos podem ser traçados usando o procedimento CinrclePoints.

Oito pontos simétricos em uma circunferência
```c
void CirclePoints(int x, int y, int color) {
    write_pixel(x, y, color);
    write_pixel(x, -y, color);
    write_pixel(-x, y, color);
    write_pixel(-x, -y, color);
    write_pixel(y, x, color);
    write_pixel(y, -x, color);
    write_pixel(-y, x, color);
    write_pixel(-y, -x, color);
}
```

# Algoritmo do "Ponto-Médio" para circunferências

Consideremos apenas um arco de 45° da circunferência, o 2° octante, de x = 0, y = R, a x = y = R/(raizQuadrada(2)), e usaremos o procedimento CirclePoints para traçar todos os pontos da circunferência. Assim como o algoritmo gerador de linhas, a estratégia é selecionar entre 2 pixels na malha aquele que está mais próximo da circunferñecia, avaliando-se uma função no ponto intermediário entre os dois pixels. No segundo octante, se o pixel P em (xp, yp) foi previamente escolhido como o mais próximo da circunferência, a escolha no próximo pixel será entre os pixels E e SE.

Seja a função F(x, y) = x^2 + y^2 - R^2, cujo valor é 0 sobre a circunferência, positivo gora dela e negativo dentro. Se o ponto intermediário (o "ponto médio") entre os pixels E e SE está fora da circunferência, o pixel Se é escolhido, porque está mais próximo dela. Por outro lado, se o pixel intermediário está dentro da circunferência, então o pixel E é escolhido.

Assim como no caso das linhas, a escolha é feita com base na variável de decisão d, que dá o valor da função no "ponto-médio":

d_old = F(xp + 1, yp - 1/2) = (xp + 1)^2 + (yp-1/2)^2 - R^2

se d_old < 0, E é escolhido, e o próximo ponto-médio será incrementado de 1 na direção x. Assim,
d_new = F(x_p + 2, y_p - 1/2) = (x_p + 2)^2 + (y_p-1/2)^2 - R^2
e d_new = d_old + (2xp + 3); consequentemente deltaE = 2xp + 3.
Se d_old >= 0, SE é escolhido, e o próximo ponto-médio será incrementado de 1 na direção x e decrementado de 1 na direção y. Portanto:
d_new = F(xp + 2, yp - 3/2) = (xp + 2)^2 + (yp - 3/2)^2 - R^2

Como d_new = d_old + (2xp - 2yp + 5), delta SE = 2xp - 2yp + 5
Resumindo, os mesmos dois passos executados para o algoritmo do traçado de linhas são executados para o algoritmo de circunferências:
1. Escolher o pixel com base no sinal da variável d, calcualda na iteração anterior;

2. Atualizar a variável d com o valor correspondente ao pixel escolhido. A diferença é que, na atualização de d, calculamos uma função linear do ponto de avaliação.

Falta ainda calcular a condição inicial (o 1º valor de d). Limitando a utilização do algoritmo a raios inteiros no segundo octante, o pixel inicial é dado por (0, R). O próximo "ponto-médio" está em (1, R - 1/2), e portanto F(1, R - 1/2) = 1 + (R^2 - R + 1/4) - R^2 = 5/4 - R. O algoritmo, bastante parecido com o algoritmo para traçado de retas, é mostrado no algoritmo :
```c
void MidPointCircle(int r, int color) {
    int x, y;
    float d;

    /* Valores iniciais */
    x = 0;
    y = r;
    d = 5/4 - r;

    CirclePoints(x, y, color);
    
    while (y > x) {
        if (d < 0) {
            /* Selecione R */
            d = d + 2 * x + 3;
            x++;
        } else {
            /* Selecione SE */
            d = d + 2 * (x - y) + 5;
            x++;
            y--;
        }

        CirclePoints(x, y, color);
    }
}
```

Para eliminar as frações, vamos definir uma nova variável de decisão, h, onde h = d - 1/4, e substituir d por h + 1/4 no código. Agora, a inicialização é h = 1 - R, e a comparação d < 0 torna-se h < -1/4. Entretanto, como o valor inicial de h é inteiro, e a variável é incrementada de valores inteiros (delta E e delta SE), a comparação pode ser alterada simplesmente para h < 0. Temos agora um algoritmo que opera em termos de h, usando apenas aritmética inteira.

# Conversão matricial de Elipses
Consideremos a elipse centrda em (0, 0) é descrita pela equação:
F(x, y) = (b^2)*(x^2) + (a^2)(y^2) - (a^2)(b^2) = 0
onde 2a é o comprimento do eixo maior (eixo x) e 2b é o comprimento do eixo menor (eixo y).

Como foi feito para circunferências, tomar-se-á como base elipses centradas na origem (translação!). As elipses possuem simetria horizontal e vertical, assim a preocupação será de traçar apenas o primeiro quadrante da elipse e depois traçar os demais por reflexão.

Primeiramente dividiremos o primeiro quadrante em regiões: o limite entre as duas regiões é o ponto da curva cuja tangente tem inclinação igual a -1. A determinação deste ponto não é tão simples.

Assim, se o próximo "ponto-médio" é a^2 (yp-1/2) <= b^2 (xp + 1), nós mudadmos da região 1 para a região 2.
Como nos outros algoritmos de "ponto-médio" anteriores, o sinal da variável de decisão d (é o valor da função no "ponto-médio") será usado para verificar que pixels fazem ou não parte da elipse.

Assim, na região 1, se o pixel corrente está em (xp, yp), então a variável de decisão para a região 1, d1, é F(xp + 1, yp - 1/2), o ponto médio entre E e SE. Repetindo o processo de derivação para a função de incremento (como foi feito para circunferência), tem-se que delta E = b^2 (2xp + 3) e delta SE = b^2 (2xp + 3) + a^2 (-2yp - 1).

Para a região 2, se o pixel corrente está em (xp, yp), então a variável de decisão para a região 2, d2, é F(xp + 1/2, yp - 1), o ponto médio entre S e SE. Pode-se fazer cálculos idênticos aos da região 1. A condição inicial deve ser então computada. Assumindo valores inteiros para a e b, a elipse começa em (0, b) e o primeiro "ponto-médio" é calculado em (1, b-1/2). Assim,

F(1, b - 1/2) = b^2 + a^2 (b - 1/2)^2 - a^2 b^2 = b^2 + a^2(-b + 1/4)

```c
void MidPointElipse(int a, int b, int color) {
    int x, y;
    float d1, d2;

    // Valores iniciais
    x = 0;
    y = b;
    d1 = b * b - a * a * b + a * a / 4.0;

    elipsePoints(x, y, color); // simetria de ordem 4

    while (a + a * (y - 0.5) > b * b * (x + 1)) {
        //Região 1
        if (d1 < 0) {
            d1 = d1 + b * b * (2 *+* x + 3);
            x++;
        } else {
            d1 = d1 + b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            x++;
            y--;
        }

        elipsePoints(x, y, color);
    }

    d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b;

    while (y > 0) {
        // Região 2
        if (d2 < 0) {
            d2 = d2 + b * b * (2 * x  + 2) + a * a * (-2 * y + 3);
            x++;
            y--;
        } else {
            d2 = d2 + a * a * (-2 * y + 3);
            y--;
        }

        elipsePoints(x, y, color);
    }
}
```
