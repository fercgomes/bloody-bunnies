# Fundamentos de Computação Gráfica
## Trabalho Final
### Datas
- **6 de maio de 2019**: entrega do nome das duplas
- **23 de junho de 2019**: [Entrega no Moodle](https://moodle.inf.ufrgs.br/mod/assign/view.php?id=81136)

### Especificações
* Você pode utilizar a linguagem de programação de sua escolha.
- Recomenda-se, entretanto, o uso da linguagem C++ com OpenGL. Isto possibilitará a reutilização de código desenvolvido em nossas aulas práticas (laboratórios). Também, facilitará na criação de uma aplicação de alta performance, com interação em tempo real.
- Você poderá utilizar no máximo 30% de código pronto para este trabalho (sem considerar o código que desenvolvemos nas aulas práticas). Qualquer utilização de código além desse limite será considerada plágio e o trabalho correspondente receberá nota zero.

* A sua aplicação deve possibilitar interação em tempo real.
- Por exemplo, se você desenvolver um jogo, ele não pode ser "lento" a ponto de impactar negativamente a jogabilidade.

* A sua aplicação deve possuir algum objetivo e lógica de controle não-trivial.
- Por exemplo, um jogo de computador possui uma lógica não-trivial. Mas, uma aplicação que simplesmente carrega um modelo geométrico 3D e permite sua visualização é trivial.

* A sua aplicação deve utilizar as matrizes que vimos em aula para transformações geométricas (Model matrix), projeções (Projection matrix), e especificação do sistema de coordenadas da câmera (View matrix).
- Você não pode utilizar bibliotecas existentes para o cálculo de câmera, transformações, etc. Por exemplo, as funções a seguir, comumente utilizadas em tutoriais disponíveis na Web, não podem ser utilizadas:
- gluLookAt(), gluOrtho2D(), gluPerspective(), gluPickMatrix(), gluProject(), gluUnProject(), glm::lookAt(), glm::ortho(), glm::perspective(), glm::pickMatrix(), glm::rotate(), glm::scale(), glm::translate(), dentre outras.
- Você pode reutilizar o código desenvolvido em nossas aulas práticas.

* A sua aplicação deve possibilitar interação com o usuário através do mouse e do teclado.

* A qualidade da apresentação do trabalho final, além da presença da dupla nos dias de apresentações de outros colegas, irá contar para a nota final do trabalho. Cada integrante da dupla irá receber pontuação independente de participação. Qualquer tipo de plágio acarretará nota zero.
A sua aplicação deve incluir implementação dos seguintes conceitos de Computação Gráfica:

* Objetos virtuais representados através de malhas poligonais complexas (malhas de triângulos).
- No mínimo sua aplicação deve incluir um modelo geométrico da complexidade igual ou maior que o modelo "cow.obj" disponível neste link.
- Para carregar este (e outros) modelos geométricos no formato OBJ, você pode utilizar bibliotecas existentes (por exemplo: tinyobjloader (C++) e tinyobjloader (C)).
- Quanto maior a variedade de modelos geométricos, melhor.

* Transformações geométricas de objetos virtuais.
- Através da interação com o teclado e/ou mouse, o usuário deve poder controlar transformações geométricas aplicadas aos objetos virtuais (não somente controle da câmera).

* Controle de câmeras virtuais.
- No mínimo sua aplicação deve implementar uma câmera look-at e uma câmera livre, conforme praticamos no Laboratório 2.

* No mínimo um objeto virtual deve ser copiado com duas ou mais instâncias, isto é, utilizando duas ou mais Model matrix aplicadas ao mesmo conjunto de vértices.
- Como exemplo, veja o código do Laboratório 2 e Laboratório 3, onde o mesmo modelo geométrico (cubo) é utilizado para desenhar todas as partes do boneco, e somente as matrizes de modelagem (Model matrix) são alteradas para desenhar cada cópia do cubo.

* Testes de intersecção entre objetos virtuais.
- No mínimo sua aplicação deve utilizar três tipos de teste de intersecção (por exemplo, um teste cubo-cubo, um teste cubo-plano, e um teste ponto-esfera).
- Estes testes devem ter algum propósito dentro da lógica de sua aplicação.
- Por exemplo, em um jogo de corrida, o modelo virtual de um carro não pode atravessar a parede, e para tanto é necessário testar a intersecção entre estes dois objetos de modo a evitar esta intersecção.

* Modelos de iluminação de objetos geométricos.
- No mínimo sua aplicação deve incluir objetos com os seguintes modelos de iluminação: difusa (Lambert) e Blinn-Phong.
- No mínimo sua aplicação deve incluir objetos com os seguintes modelos de interpolação para iluminação:
- No mínimo um objeto com modelo de Gouraud: o modelo de iluminação é avaliado para cada vértice usando suas normais, gerando uma cor, a qual é interpolada para cada pixel durante a rasterização.
- No mínimo um objeto com modelo de Phong: as normais de cada vértice são interpoladas para cada pixel durante a rasterização, e o modelo de iluminação é avaliado para cada pixel, utilizando estas normais interpoladas.

* Mapeamento de texturas.
- No mínimo um objeto virtual de sua aplicação deve ter sua cor definida através de uma textura definida através de uma imagem.

* Curvas de Bézier.
- No mínimo um objeto virtual de sua aplicação deve ter sua movimentação definida através de uma curva de Bézier cúbica. O objeto deve se movimentar de forma suave ao longo do espaço em um caminho curvo (não reto).

* Animação de Movimento baseada no tempo.
- Todas as movimentações de objetos (incluindo da câmera) devem ser computadas baseado no tempo (isto é, movimentações devem ocorrer sempre na mesma velocidade independente da velocidade da CPU onde o programa está sendo executado).

### Entrega
Um dos integrantes da dupla deve fazer envio no Moodle de um arquivo ZIP com o seguinte conteúdo:

* Código fonte completo documentado;
* Binário compilado para Windows ou para Linux (Ubuntu), incluindo todas as bibliotecas necessárias para execução;
* Relatório no formato PDF, contendo uma descrição simples sobre o desenvolvimento do trabalho, o qual deve obrigatoriamente incluir:
* Descrição do processo de desenvolvimento e do uso em sua aplicação dos conceitos de Computação Gráfica estudados e listados nos requisitos acima;
* No mínimo três imagens mostrando o funcionamento da aplicação;
* Um manual descrevendo a utilização da aplicação (atalhos de teclado, etc.);
* Explicação de todos os passos necessários para compilação e execução da aplicação;
* Slides que a dupla irá utilizar para apresentação do trabalho nas aulas dos dias 24 e 26 de junho de 2019 (formato PDF, Powerpoint, ou OpenDocument). A ordem de apresentações se dará por sorteio nestes mesmos dias.
* A entrega deve ser feita impreterivelmente até 23 horas e 55 minutos do dia 23 de junho de 2019, utilizando o botão "Adicionar tarefa" abaixo.
* Caso necessário, separem seus arquivos ZIP em partes de 100 MB cada. O Moodle aceita upload de até 2 arquivos de 100 MB, por aluno.

