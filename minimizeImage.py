from PIL import Image

def resize_image(input_path, output_path, max_size=120):
    try:
        # Abrir a imagem
        with Image.open(input_path) as img:
            # Obter as dimensões originais
            original_width, original_height = img.size
            print(f"Tamanho original: {original_width}x{original_height}")
            
            # Calcular o novo tamanho mantendo a proporção
            if original_width > original_height:
                scale_factor = max_size / original_width
            else:
                scale_factor = max_size / original_height

            new_width = int(original_width * scale_factor)
            new_height = int(original_height * scale_factor)

            # Redimensionar a imagem
            resized_img = img.resize((new_width, new_height))
            print(f"Tamanho redimensionado: {new_width}x{new_height}")

            # Salvar a imagem redimensionada
            resized_img.save(output_path)
            print(f"Imagem salva em: {output_path}")
    except Exception as e:
        print(f"Erro ao processar a imagem: {e}")

# Caminhos da imagem
input_image_path = "images/man.ppm"  # Caminho relativo para a pasta 'images'
output_image_path = "images/manMenor.ppm"  # Caminho relativo para salvar a imagem redimensionada

# Redimensionar a imagem
resize_image(input_image_path, output_image_path)
