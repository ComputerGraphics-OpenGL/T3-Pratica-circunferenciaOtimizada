#include <GL/glut.h>
#include <math.h>

// Função que desenha os oito pontos simétricos de uma circunferência
void CirclePoints(int x, int y, int color) {
    glColor3f(color, color, color); // Define a cor do pixel
    glBegin(GL_POINTS); // Inicia o modo de desenho de pontos
    glVertex2i(x, y); // Desenha o pixel (x, y)
    glVertex2i(x, -y); // Desenha o pixel (x, -y)
    glVertex2i(-x, y); // Desenha o pixel (-x, y)
    glVertex2i(-x, -y); // Desenha o pixel (-x, -y)
    glVertex2i(y, x); // Desenha o pixel (y, x)
    glVertex2i(y, -x); // Desenha o pixel (y, -x)
    glVertex2i(-y, x); // Desenha o pixel (-y, x)
    glVertex2i(-y, -x); // Desenha o pixel (-y, -x)
    glEnd(); // Finaliza o modo de desenho de pontos
}

// Função que implementa o algoritmo do ponto-médio para traçar uma circunferência
void MidPointCircle(int x_centre, int y_centre, int r, int color) {
    int x = 0, y = r; // Valores iniciais
    int d = 1 - r; // Valor inicial da variável de decisão
    CirclePoints(x + x_centre, y + y_centre, color); // Desenha os pontos iniciais
    while (x < y) { // Enquanto x for menor que y
        x++; // Incrementa x em uma unidade
        if (d < 0) { // Se o ponto médio está dentro da circunferência
            d = d + 2 * x + 1; // Atualiza a variável de decisão
        } else { // Se o ponto médio está fora da circunferência
            y--; // Decrementa y em uma unidade
            d = d + 2 * (x - y) + 1; // Atualiza a variável de decisão
        }
        CirclePoints(x + x_centre, y + y_centre, color); // Desenha os pontos simétricos
    }
}

// Função que desenha uma cena
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cor
    MidPointCircle(0, 0, 100, 1); // Desenha uma circunferência de raio 100 e cor branca
    glFlush(); // Força o desenho dos comandos
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Define o modo de exibição
    glutInitWindowSize(650, 650); // Define o tamanho da janela
    glutInitWindowPosition(100, 100); // Define a posição inicial da janela
    glutCreateWindow("Algoritmo do Ponto-Medio para Circunferencias"); // Cria a janela com um título
    glClearColor(0.0, 0.0, 0.0, 0.0); // Define a cor de fundo como preta
    gluOrtho2D(-250, 250, -250, 250); // Define o sistema de coordenadas ortogonal
    glutDisplayFunc(display); // Registra a função de desenho
    glutMainLoop(); // Entra no loop principal do GLUT
    return 0;
}
