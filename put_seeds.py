import cv2
import numpy as np

def carregar_ppm(caminho):
    """Carrega uma imagem no formato PPM (P6)."""
    with open(caminho, 'rb') as arquivo:
        # Verifica o formato
        tipo = arquivo.readline().strip()
        if tipo != b'P6':
            raise ValueError("Formato PPM inválido! Apenas P6 é suportado.")

        # Ignora comentários
        linha = arquivo.readline()
        while linha.startswith(b'#'):
            linha = arquivo.readline()

        # Lê dimensões da imagem
        largura, altura = map(int, linha.split())

        # Lê o valor máximo de cor (assumimos 255)
        max_valor = int(arquivo.readline().strip())
        if max_valor != 255:
            raise ValueError("Apenas imagens com valor máximo de cor 255 são suportadas.")

        # Lê os pixels
        pixels = np.frombuffer(arquivo.read(), dtype=np.uint8)
        return largura, altura, pixels

def salvar_ppm(caminho, largura, altura, pixels):
    """Salva uma imagem no formato PPM (P6)."""
    with open(caminho, 'wb') as arquivo:
        arquivo.write(b"P6\n")
        arquivo.write(f"{largura} {altura}\n".encode())
        arquivo.write(b"255\n")
        arquivo.write(pixels.tobytes())

def pintar_com_interacao(caminho_imagem, caminho_saida, caminho_txt):
    """Permite que o usuário pinte áreas da imagem e salva o resultado."""
    largura, altura, pixels = carregar_ppm(caminho_imagem)

    # Converte pixels para uma imagem OpenCV
    imagem = pixels.reshape((altura, largura, 3))
    imagem = cv2.cvtColor(imagem, cv2.COLOR_RGB2BGR)


    if largura > 300 or altura > 300:
        escala = 1  # Não redimensiona
    else:
        escala = 5 
    imagem = cv2.resize(imagem, (largura * escala, altura * escala), interpolation=cv2.INTER_NEAREST)

    # Lista para armazenar posições pintadas
    posicoes_pintadas = []

    def draw_circle(event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN or (event == cv2.EVENT_MOUSEMOVE and flags & cv2.EVENT_FLAG_LBUTTON):
            cor, codigo = param
            raio = 7
            cv2.circle(imagem, (x, y), raio, cor, -1)
            for dx in range(-raio, raio + 1):
                for dy in range(-raio, raio + 1):
                    if dx**2 + dy**2 <= raio**2:  # Dentro do círculo
                        px, py = (x + dx) // escala, (y + dy) // escala
                        if 0 <= px < largura and 0 <= py < altura:
                            posicoes_pintadas.append((px, py, codigo))

    # Cria janela e interage com o usuário
    cv2.namedWindow("Pintar Imagem", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("Pintar Imagem", largura * escala, altura * escala)
    cv2.setMouseCallback("Pintar Imagem", draw_circle, ((0, 0, 255), 0))  # Vermelho por padrão

    while True:
        cv2.imshow("Pintar Imagem", imagem)
        tecla = cv2.waitKey(1) & 0xFF

        if tecla == ord('o'):  # Muda para vermelho
            cv2.setMouseCallback("Pintar Imagem", draw_circle, ((0, 0, 255), 0))
        elif tecla == ord('b'):  # Muda para verde
            cv2.setMouseCallback("Pintar Imagem", draw_circle, ((0, 255, 0), 1))
        elif tecla == 27:  # ESC para sair
            break

    cv2.destroyAllWindows()

    # Redimensiona de volta a imagem para o formato original
    imagem = cv2.resize(imagem, (largura, altura), interpolation=cv2.INTER_AREA)

    # Converte imagem de volta para o formato PPM
    imagem_ppm = cv2.cvtColor(imagem, cv2.COLOR_RGB2BGR).flatten()
    salvar_ppm(caminho_saida, largura, altura, imagem_ppm)

    # Salva as posições pintadas
    with open(caminho_txt, 'w') as arquivo:
        for x, y, codigo in posicoes_pintadas:
            arquivo.write(f"{x} {y} {codigo}\n")

if __name__ == "__main__":
    # Caminho da imagem original
    nome_image = input("Digite o nome da imagem: (sem o ppm): ")
    caminho_imagem = "images/" + nome_image + ".ppm"

    # Caminho para salvar a nova imagem
    caminho_saida = nome_image + "_pintada.ppm"

    # Caminho para salvar o arquivo de posições
    caminho_txt = "Parte2/seeds/seeds_" + nome_image + ".txt"

    pintar_com_interacao(caminho_imagem, caminho_saida, caminho_txt)
    print("Processo concluído! Imagem pintada e posições salvas.")
