# WebOs SuperTux

Port de [SuperTux 0.6.3](https://github.com/SuperTux/supertux/tree/v0.6.3)
para TVs LG com webOS e acesso a aplicativos nativos. O jogo usa SDL2 e
OpenGL ES 2. A versão testada é `matias.supertux` 0.6.6, em uma TV LG webOS
com acesso root.

## Controle remoto

As setas movimentam Tux e navegam pelos menus. OK seleciona nos menus e pula
durante a partida. Em **Options > Setup Keyboard**, selecione uma ação com OK
e pressione o botão do controle que deseja atribuir. O mapeador de teclado do
próprio SuperTux salva as atribuições no perfil do jogo.

| Botão | Código Linux observado | Ação inicial |
| --- | ---: | --- |
| Setas | 103, 108, 105, 106 | Movimento e menus |
| OK | 28 | Pular e selecionar |
| Voltar | 412 | Voltar |
| Vermelho | 398 | Ação |
| Verde | 399 | Pausa/menu |
| Amarelo | 400 | Olhar à esquerda |
| Azul | 401 | Olhar à direita |
| Play | 207 | Pular |
| Pause | 119 | Pausa/menu |
| Retroceder | 168 | Olhar à esquerda |
| Avançar | 208 | Olhar à direita |
| Stop | 128 | Voltar |
| OK do ponteiro | 272 | Pular e selecionar |
| 8 / 0 | 9 / 11 | Olhar acima / abaixo |

Os códigos foram observados em `/dev/input/event3` da TV testada. Alguns
botões não chegam diretamente ao SDL. O patch lê os dispositivos RCU,
transforma esses eventos em teclas SDL F13–F23 e os entrega ao mapeador do
jogo. O nome do botão aparece na tela de configuração depois da atribuição.
Botões reservados pelo sistema da TV podem não chegar ao aplicativo.

## Arquivos

- `webos-tv.patch`: alterações sobre o código oficial do SuperTux 0.6.3.
- `launcher.c`: inicia o jogo com bibliotecas locais e ativa leitura do remoto.
- `package/appinfo.json`: manifesto do aplicativo webOS.
- `package/icon.png`: ícone do aplicativo.

Binários, SDK, bibliotecas e dados do jogo são artefatos de compilação; não
ficam no Git. O jogo completo usa os dados da distribuição oficial do
SuperTux.

## Compilação

É necessário um SDK ARM compatível com a TV, CMake, SDL2 e as demais
dependências do SuperTux 0.6.3. A compilação testada usou um SDK Buildroot
`arm-webos-linux-gnueabi`, Boost, OpenAL Soft, Vorbis e OpenGL ES 2.

```sh
git clone --recursive --branch v0.6.3 https://github.com/SuperTux/supertux.git
cd supertux
git apply ../WebOs-SuperTux/webos-tv.patch
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/caminho/do/sdk/share/buildroot/toolchainfile.cmake \
  -DCMAKE_BUILD_TYPE=Release -DENABLE_OPENGL=ON -DENABLE_OPENGLES2=ON \
  -DBUILD_TESTS=OFF -DBUILD_DOCUMENTATION=OFF
cmake --build build -j 8
```

Para empacotar, compile `launcher.c` para ARM como `package/run-supertux`,
copie `LICENSE.txt` para `package/` e coloque `build/supertux2`, `data.zip`
com os dados do jogo e as bibliotecas necessárias em `package/`.
Inclua em `data.zip` o arquivo gerado `build/data/levels/misc/menu.stl`;
somente `data/levels/misc/menu.stl.in` não inicia o menu. Depois use
`ares-package -A arm -o . package` e `ares-install` para instalar o IPK.
O [tutorial oficial de aplicativos nativos
integrados](https://www.webosose.org/docs/tutorials/native-apps/developing-built-in-native-apps/)
descreve o ambiente webOS OSE; esta porta foi testada em uma TV LG webOS.

## Licença

O SuperTux e este port seguem a GPL-3.0-or-later. Consulte [LICENSE.txt](LICENSE.txt)
e os avisos de licença no código original e nas dependências.
