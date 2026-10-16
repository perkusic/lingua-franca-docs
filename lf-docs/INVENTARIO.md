# Inventário estrutural dos três HTMLs

Gerado em 2026-10-15 (atualizado em 16/out) por uma rotina que lê os três arquivos em disco. O gerador de 8/out
não está na pasta; a rotina de 15/out foi reescrita e **calibrada** nos arquivos de 8/out:
reproduz todos os números daquela versão, exceto as palavras (método diferente). Por isso as
palavras das duas edições de LF são as de 8/out somadas ao delta medido pela rotina nova, nos
arquivos antigo e novo (mesmo método, precedente de 29/set). A especificação HW-RTI mudou só em texto
em 15/out (correções no histórico): caracteres, bytes e palavras (pelo delta) remedidos; o resto da coluna é o de 8/out. Serve como base de conferência: se um handoff do
projeto **LinguaFranca** afirmar um número diferente, o handoff é que está
defasado. Ao mudar qualquer HTML, regenere o inventário inteiro em vez de
corrigir linhas isoladas.

Arquivos medidos (SHA-256, 16 primeiros dígitos):

| Arquivo | SHA-256 |
| --- | --- |
| `tutorial/lingua_franca_rti.html` | `4dfb0ad5a06423b1` |
| `tutorial/lingua_franca_tutorial.html` | `9993dd1835f674e4` |
| `hw-rti/HW-RTI_project_specification.html` | `f437eacd756bc8d0` |

## 1. Números gerais

| | `lingua_franca_rti.html` | `lingua_franca_tutorial.html` | `HW-RTI_project_specification.html` |
| --- | --- | --- | --- |
| Caracteres | 618.782 | 801.377 | 259.726 |
| Bytes (UTF-8) | 618.808 | 801.491 | 259.738 |
| Palavras de texto (sem SVG, CSS) | 40.634 | 55.413 | 22.761 |
| h1 / h2 / h3 / h4 | 1 / 17 / 48 / 7 | 1 / 17 / 48 / 7 | 1 / 15 / 44 / 5 |
| Figuras (`<figure>` = `<svg>`) | 44 | 44 | 12 |
| Figuras animadas (`<svg class="anim">`) | 29 | 29 | 5 |
| Tabelas | 12 | 12 | 32 |
| Itens de lista | 226 | 331 | 67 |
| Referências | 51 | 51 | 20 |
| Links `<a href>` | 440 | 529 | 26 |
| Caixas de checkpoint | 0 | 16 | 0 |
| Perguntas após figura ou exemplo (`.fq`) | 0 | 48 | 0 |
| Scripts inline | 2 | 5 | 1 |
| Exemplos de código (`.ex`) | 0 | 13 | 0 |
| Aberturas de parte | 0 | 5 | 0 |

`<section>` não é usado em nenhum dos três; a estrutura é dada pelos headings.

## 2. Dependências externas

Os três arquivos carregam fontes do Google Fonts por `<link>` no `<head>`
(domínios: `fonts.googleapis.com`): Space Grotesk, Source Serif 4 e
IBM Plex Mono. Nenhum outro recurso externo é carregado (sem scripts, imagens ou
folhas de estilo de terceiros). Sem rede, a página continua legível: cada regra
`font-family` tem fallback (`sans-serif`, `Georgia, serif`, `monospace`), mas a
tipografia muda e as larguras de texto nas figuras SVG, medidas com IBM Plex Mono,
podem variar. Portanto os documentos **não** são totalmente autocontidos.
**Decisão de 28/set/2026: manter o Google Fonts.** A dependência é aceita e
fica registrada aqui; embutir as fontes ou trocá-las por fontes do sistema exigiria
reconferir todas as figuras.

## 3. Referência e tutorial

Similaridade do texto normalizado (palavras, sem SVG e CSS, `difflib` sem
autojunk): **0.842** (0.838 em 8/out; a rotina nova reproduz 0.838 nos arquivos de 8/out). Medições anteriores deste inventário usavam outro
método e não são comparáveis com este número.

h2 idênticos nas duas edições, salvo rótulos: sim
(17 h2). Os 48 h3 são literalmente os mesmos nas duas edições.

Remissões a figura no corpo: desde 15/out são links para o `id` estável de cada figura (contagem e destinos conferidos por `.github/scripts/check_html.py`); a lista abaixo é de 15/out, antes da Figura 1 nova, e está com a numeração antiga (some 1):
referência Figure 6, Figure 18, Figure 4, Figure 30, Figure 28, Figure 19, Figure 32, Figure 31, Figure 33, Figure 27, Figure 28, Figure 27, Figure 2, Figure 30, Figure 30, Figure 31, Figure 32, Figure 32, Figure 33, Figure 30, Figure 34, Figure 4, Figure 35, Figure 36, Figure 37, Figure 35, Figure 38, Figure 39, Figure 40, Figure 38, Figure 41, Figure 42, Figure 43, Figure 38;
tutorial Figure 2, Figure 2, Figure 9, Figure 6, Figure 18, Figure 19, Figure 4, Figure 30, Figure 28, Figure 19, Figure 32, Figure 31, Figure 33, Figure 27, Figure 28, Figure 27, Figure 2, Figure 30, Figure 30, Figure 31, Figure 32, Figure 32, Figure 33, Figure 30, Figure 30, Figure 32, Figure 30, Figure 34, Figure 4, Figure 35, Figure 36, Figure 37, Figure 35, Figure 38, Figure 39, Figure 40, Figure 38, Figure 41, Figure 42, Figure 43, Figure 38.

### Figuras — 44

| # | viewBox | Assunto | Animada |
| --- | --- | --- | --- |
| 1 | 760×452 | the actor model, message by message | sim |
| 2 | 760×330 | one message at a time is not one order | sim |
| 3 | 760×250 | a reactor, its ports, and levels |  |
| 4 | 760×330 | federation architecture |  |
| 5 | 760×404 | joining a federation | sim |
| 6 | 760×360 | the question the RTI answers |  |
| 7 | 760×300 | the grant decision |  |
| 8 | 760×420 | the two safety conditions, on the wire | sim |
| 9 | 760×524 | a zero-delay cycle, message by message | sim |
| 10 | 760×296 | superdense time |  |
| 11 | 760×546 | how the graph becomes a decision | sim |
| 12 | 760×334 | why the RTI cannot trust the NET alone | sim |
| 13 | 760×300 | the cost of one NET |  |
| 14 | 760×250 | the example federation |  |
| 15 | 760×290 | releasing the cycle |  |
| 16 | 760×210 | what DNET provides |  |
| 17 | 760×474 | where the algebra comes from |  |
| 18 | 760×450 | when absence is safe to assume | sim |
| 19 | 760×452 | two ways to establish that a tag is safe | sim |
| 20 | 760×380 | which cycles can exist | sim |
| 21 | 760×262 | the federate scheduler |  |
| 22 | 760×300 | the Reaction module |  |
| 23 | 760×360 | Codesign top level |  |
| 24 | 760×250 | the Codesign build |  |
| 25 | 760×300 | cascade and snapshot | sim |
| 26 | 760×444 | three quantities on one timeline | sim |
| 27 | 760×260 | InterPRET and the time-based rendezvous |  |
| 28 | 760×330 | four orders | sim |
| 29 | 760×350 | fixed points of a monotone function | sim |
| 30 | 760×330 | two dependency graphs of one program | sim |
| 31 | 760×262 | the closure as successive approximations | sim |
| 32 | 760×364 | why any relaxation order ends at the least delays | sim |
| 33 | 760×330 | the fixed point of one tag | sim |
| 34 | 760×300 | why an acyclic reaction graph gives exactly one behaviour at a tag | sim |
| 35 | 760×440 | what the compiler builds for Example&nbsp;16.1 | sim |
| 36 | 760×470 | startup between two neighbors, as logged | sim |
| 37 | 760×420 | the startup coordinator as a state machine | sim |
| 38 | 760×384 | the TCP network channel as a state machine | sim |
| 39 | 760×360 | one clock-synchronization round | sim |
| 40 | 760×520 | how a micro-LF federate advances, as two state machines | sim |
| 41 | 760×470 | Example 16.1 on dst’s physical timeline | sim |
| 42 | 760×360 | a tag with nothing to send, under both architectures | sim |
| 43 | 760×388 | the micro-LF decision as a datapath (interpretation) | sim |
| 44 | 760×440 | where a micro-LF program meets the hardware | sim |

O tutorial tem as mesmas figuras, na mesma ordem: sim.

### O que só o tutorial tem

Andaime pedagógico: a caixa "How to use this tutorial", um bloco "In this lesson
you will" por lição, 16 caixas de checkpoint, 48 perguntas "Read the figure"
(uma após cada figura, com resposta recolhível), 5 aberturas de parte, o
sumário agrupado por parte e dois blocos "Lab step". Nada disso cria heading.

### Figuras interativas

As figuras com `class="anim"` são **estáticas por padrão**. Um único `<script>` inline no
fim de cada HTML acrescenta, sob cada uma, os controles ◀ / Play / ▶ / All (passo a passo,
reprodução e pausa); sem o script, e na impressão, a figura aparece inteira. Nenhuma
figura se anima sozinha.

### Exemplos de código (só no tutorial)

13 blocos `<div class="ex">`: 11 de Lingua Franca, cada um com o programa em C e em
Python (abas), e o Exemplo 15.1 do apêndice, um script em Python puro (`fixpoint.py`) que calcula os
dois pontos fixos das Figuras 30 e 32, depois de conferir a soma de tags. Cada bloco diz como rodar pela linha de comando ou pelo VS Code
e traz a saída de uma execução real. Os de LF foram compilados e executados com `lfc` 0.13.0 em
28/set/2026; as fontes estão em `tutorial/examples/` (20 arquivos `.lf`, `fixpoint.py` e um README).
O 13º é o Exemplo 16.1 (micro-LF, `examples/micro-lf/Tardy.ulf`): o `.ulf` **não** foi compilado
(o `ulfc` exige Maven Central, inacessível); a saída impressa vem de `src.c`/`dst.c`, o equivalente
escrito à mão contra o runtime `reactor-uc` em `df7cfea`, executado em 15/out/2026, e o bloco diz isso.
Os três do laboratório (12.1–12.3) ficam atrás de um portão ("I have written mine"). Um `<script>`
inline faz as abas, a cópia e o portão; sem ele, todos os painéis aparecem. As duas edições têm a §14
(ferramentas: instalar, compilar, rodar, rastrear) e a §15 (apêndice de pontos fixos).

### Autoverificação (só no tutorial)

Cada pergunta de checkpoint (`<li data-q="Cb.n">`) e cada pergunta após figura
(`<div class="fq" data-q="Fn">`) tem uma definição em `<script type="application/json"
id="quiz-data">`: 103 perguntas, das quais
67 com verificação exata (50 múltipla escolha, 3 marcar todas, 8 campo numérico/tag,
6 associação) e 36 com rubrica e autoavaliação. Um `<script>` inline esconde a resposta até
uma tentativa (objetivas) ou até 20 caracteres escritos (abertas), guarda as respostas no
`localStorage` do navegador (`lf-tutorial-quiz-v1`) e mostra um placar por checkpoint. Sem
script, as respostas continuam nos `<details>`; na impressão, abrem todas.

## 4. Especificação HW-RTI

15 seções (h2) e 44 subseções (h3). Remissões a figura no corpo:
Figure 2, Figure 2.

### Identificadores

| Família | Faixa | Total | Contígua |
| --- | --- | --- | --- |
| `G` | G-1 → G-6 | 6 | sim |
| `NG` | NG-1 → NG-7 | 7 | sim |
| `FR` | FR-1 → FR-20 | 20 | sim |
| `NFR` | NFR-1 → NFR-9 | 9 | sim |
| `D` | D-1 → D-11 | 11 | sim |
| `L` | L-1 → L-6 | 6 | sim |
| `A` | A-1 → A-4 | 4 | sim |
| `UT` | UT-1 → UT-9 | 9 | sim |
| `DT` | DT-1 → DT-6 | 6 | sim |
| `FV` | FV-1 → FV-7 | 7 | sim |
| `ST` | ST-1 → ST-4 | 4 | sim |
| `PT` | PT-1 → PT-3 | 3 | sim |
| `KPI` | KPI-1 → KPI-6 | 6 | sim |
| `R` | R-1 → R-9 | 9 | sim |
| `M` | M-0 → M-9 | 10 | sim |
| `DL` | DL-1 → DL-7 | 7 | sim |
| `OQ` | OQ-1 → OQ-8 | 8 | sim |

Mais citados no corpo: DT-1 (16), G-2 (12), G-5 (11), KPI-1 (11), M-4 (11), FR-18 (10), NG-6 (9), NFR-01 (9).

### Figuras — 12

| # | viewBox | Assunto | Animada |
| --- | --- | --- | --- |
| 1 | 780×360 | the question the RTI answers |  |
| 2 | 780×450 | anatomy of the response time |  |
| 3 | 780×240 | the shape of the claim |  |
| 4 | 780×444 | three quantities on one timeline | sim |
| 5 | 780×380 | system context |  |
| 6 | 780×420 | internal microarchitecture |  |
| 7 | 780×300 | cascade and snapshot | sim |
| 8 | 780×322 | why the data plane must not race the LTC |  |
| 9 | 780×318 | the sequencer | sim |
| 10 | 780×300 | decision unit datapath |  |
| 11 | 780×300 | one message through the pipeline, n = 16, d = 4 | sim |
| 12 | 780×400 | what passing the equivalence test means | sim |

## 5. Histórico que explica números antigos

- 18/set/2026: reorganização em `tutorial/` e `hw-rti/`; a cópia da
  especificação que foi para `hw-rti/` era anterior ao §11 e foi restaurada.
- 28/set/2026: arquivos residuais da raiz e as pastas `chatgpt/`,
  `Claude outputs/` e `_antigo/` apagados; §4.14 e 9 figuras novas nos
  documentos de LF (5 de conteúdo novo e a Figura 4, que é a Figura 1 da
  especificação adaptada); §1.4, §5.2, §5.3 e 5 figuras novas na especificação;
  correções de ciclos de atraso zero, laboratório, contrato de latência,
  vazão, FV-1 e contrato de equivalência; na segunda revisão do mesmo dia,
  FV-1 e FV-3 por ramo e com limites em ciclos, merge pelo grant mais forte
  (K_max = K + (n−1)·z), preâmbulo do DT-1, RESIGN na especificação e G-3/KPI-3
  separando tempo fixo por configuração de independência do tamanho. Ainda em
  28/set (noite): 5 figuras novas de raciocínio passo a passo nos documentos de LF
  (grafo → EIMT, fila em trânsito, janela de ausência, ciclos permitidos e
  rejeitados, K/K_max/I) e 1 na especificação (K/K_max/I), a Figura 11 da
  especificação refeita como Figura 12 (harness expandido), animações trocadas por
  controles opcionais e uma pergunta após cada figura do tutorial. Na mesma noite, as 63
  perguntas do tutorial ganharam autoverificação (33 exatas, 30 por rubrica). Depois: 11 exemplos de código em C e
  Python (compilados com lfc 0.13.0), 4 perguntas "Predict before you run", a §14 de ferramentas
  nas duas edições e quatro correções técnicas descobertas ao compilar: níveis da Figura 1
  (reações de um reator são sequenciais), STA padrão *forever* com `@maxwait`, handler
  `tardy`, e a mensagem real do lfc para laços de causalidade. Revisão de 29/set/2026:
  definição de PTAG refeita nas três documentações (reações com entradas conhecidas rodam e
  enviam saídas; o tag só se completa quando todas as entradas estão resolvidas), roteiro
  Windows/WSL passo a passo na §14.1, `pkill` trocado por `lsof`/`kill` do processo certo,
  P4 e nota do Exemplo 8.1 sem prometer resultado independente do sistema operacional. Ainda em 29/set: §1.1
  (atores e reatores, ordenação de mensagens) nas duas edições, com a Figura 1 nova (as demais
  +1), refs 41–44 (Hewitt 1973, Agha 1986, Lee et al. 2003, vídeo Lang.NEXT 2012) e DOIs em
  [1] e [2]; no tutorial, C1.3–C1.5 e FA1 no quiz. Depois: Figura 4 (entrada na
  federação) refeita como sequência em 9 passos, com UDP_PORT e sincronização inicial de relógio,
  e uma lista numerada de notas (`ol.figsteps`) que acompanha o passo corrente. Em seguida, notas passo a
  passo em todas as figuras animadas: 10 em cada edição de LF e 5 na especificação (a do
  sequenciador e a do pipeline ganharam passos para isso); o passo corrente aparece numa caixa
  sob os controles, e a lista completa fica depois da figura. Abertura do §4 expandida: o
  "cálculo de ponto fixo puramente reativo" explicado em três níveis, com as exceções. Depois:
  §15 (apêndice "Fixed points, dependency graphs and federated execution") nas duas edições de LF,
  com 15.1–15.4, esboço de prova recolhível, Figuras 26–28 passo a passo, refs 45–47 (Tarski 1955,
  Tarjan 1972, Davey–Priestley 2002); no tutorial, FB1–FB3, C15.1–C15.4 e o Exemplo 15.1
  (`fixpoint.py`, Python puro). Em seguida, duas figuras para a 15.1 (26, quatro ordens em diagramas
  de Hasse; 27, pontos fixos de uma função monótona e a iteração de Kleene), as do apêndice passando a
  28–30, e FB4–FB5 no quiz. Na tarde de 29/set, revisão externa aplicada nas duas edições:
  `tag_add` do `fixpoint.py` segue `lf_tag_add` (microstep do segundo operando) e o programa
  confere casos com microstep; portas em três estados, com reações não disparadas; §15.3 passou
  a "Two fixed points and a safe advance"; legendas das Figuras 26, 27, 29 e 30 revistas; ref. [5]
  com os commits `a852b20` e `ccd0512`. Ainda em 29/set: abertura da §15 reescrita com rigor
  (três afirmações numeradas, com hipóteses e o tipo de justificativa de cada uma) e dois
  esboços de prova novos na §15.3 (qualquer ordem de relaxação termina no fecho; grafo de reações
  acíclico dá um único ponto fixo por tag); números gerais das duas edições de LF remedidos
  (palavras pelo delta, mesmo método). Na mesma noite, duas figuras para os esboços de prova da
  §15.3: a nova Figura 30 (qualquer ordem de relaxação chega aos menores atrasos, num grafo de quatro
  nós, 5 passos) e a Figura 32 (grafo acíclico ⇒ um só ponto fixo por tag, 4 passos); a antiga 30 virou
  31. No tutorial, FB6 e FB7 (objetivas) no quiz. Depois, registro formal nos três arquivos:
  "comfort"/"comfortable"/"uncomfortable" substituídos (pré-requisitos do tutorial, a trava recursiva
  em §4.12 e duas frases da especificação, §11 e §13). Em seguida, revisão de registro nos três arquivos: cerca de 240 trocas de
  vocabulário informal (get/gets, just below, cheap, nobody, honest, buys, survives, sits/lives,
  knob, spine, shortcut, story…), com o termo "cheap condition" renomeado para "low-cost
  condition" (também no rótulo da Figura 7). Depois, revisão de precisão pedida pelo Angelo: tags
  explícitas no lugar de "immediately below N ms" (predecessor definido em §4.4), regra do DNET
  descrita pela condição do código (NET só para g > X), seis frases reescritas e o texto do quiz
  (F4, F6, F9, F10, FB1, C4.3) alinhado. No tutorial, `lfc` definido no primeiro uso
  ("How to use" e "Try it") e as cinco remissões a "§14" viraram link para o Toolchain appendix. Em 30/set, sumário lateral fixo nas duas edições de LF
  (script inline novo; só em telas com 1180 px ou mais, nunca na impressão). Terceira rodada de registro: verbos de ação e de fala aplicados a coisas
  ("the lessons form", "says nothing", "tells the RTI", "asks to advance", "supply") trocados por verbos
  precisos nos três arquivos; ver `REVISAO_REGISTRO.md`. Revisão acadêmica de 30/set (A01–A14) aplicada nos três arquivos, com
  `fixpoint.py` e o Exemplo 15.1 (relaxação F(D) = min(E, D ⊗ E) a partir do ⊥). Em 7/out, §5.3 nova nas duas edições de LF (micro-LF / `reactor-uc`:
  coordenação descentralizada sem RTI, `TaggedMessage`, `last_known_tag`, `@maxwait`, violação de
  safe-to-process e `tardy`), com a Figura 18 nova (as antigas 18–32 viraram 19–33), refs 48–51 e uma
  frase em §9.9; no tutorial, objetivo novo na lição 5, C7.4–C7.5 no checkpoint da lição e FC1 após a
  figura. Antes de a especificação ser editada, suas contagens refeitas por este gerador já diferiam do
  inventário anterior (palavras 21.014 → 21.000; KPI-1 citado 9 → 11 vezes), por causa do método. Na mesma
  data, a especificação ganhou a alternativa descentralizada: termo "Centralized / decentralized" em §0.2, NG-4
  reescrita, linha nova na tabela de §1.4 (micro-LF / `reactor-uc`), OQ-7, R-9 e duas referências em §14.3.
  Depois, §5.3 e a linha de §1.4 corrigidas: o anúncio de entrada de um federado também fixa `last_known_tag`
  (terceira fonte de ausência; defeito aparente no laço que faz a escrita), e desserialização falha ou fila cheia
  perdem o evento mas avançam `last_known_tag`. Em 8/out/2026, a especificação passou à revisão 0.2: alvo
  ZCU106 (XCZU7EV -2; Zynq-7020 mantido como referência de área), G-4 reescrita, FR-20 (adaptador de porta
  para o reactor-chisel em `d94dd2d`: tag de 32 bits sem microstep, sem PTAG, TAG ≤ NET), D-11, UT-9, OQ-8,
  §8.1 com as três incompatibilidades, linha XCZU7EV em §9.3 e uma referência nova em §14.3 (20 no total).
  No mesmo dia, `program/implementation_guide.html` substituiu a visão geral do programa. Qualquer
  contagem citada em outro lugar com data anterior deve ser conferida contra este arquivo.
- 15/out/2026: Lição 16 / §16 nova nas duas edições de LF, no fim do documento (Parte V, "Beyond the
  RTI"; nada renumerado): micro-LF sem RTI, com 16.1–16.6 (do programa aos federados; startup observado;
  um tag passo a passo; custo da ausência e escolha de W; datapath por federado (interpretação);
  plataformas e requisitos de hardware, com tabela das nove plataformas do `reactor-uc` em `df7cfea`),
  dez figuras novas (34–43: anatomia, MSC de startup, máquinas de estado do startup, do canal TCP e do
  escalonador, MSC de sincronização de relógio, linha do tempo medida, MSC de tag silencioso nas duas
  arquiteturas, datapath, camadas/porte), duas tabelas e uma frase em §5.3 apontando para a lição. No
  tutorial: Exemplo 16.1, objetivos, checkpoint C16.1–C16.7, FD1–FD10, "How to use" com cinco partes.
- 15/out/2026 (mais tarde), depois de uma avaliação externa (ChatGPT): verbete STA / STAA do glossário das duas
  edições de LF distingue reactor-c (STA/STAA/STP, `@absent_after`) de micro-LF (só `@maxwait`, sem mensagem de
  ausência) e "último tag recebido" de "nenhuma mensagem anterior pode chegar". Especificação: G-3, L-2, D-3 e a
  linha "Notification fan-out" de §11.1 dizem o que a avaliação paralela não remove (distribuição de estado,
  interconexão e arbitragem, caminho crítico, atualização consistente dos registradores; medidos no M-2/PT-2/KPI-3);
  L-2 não diz mais que o trabalho uniforme "is free"; os 150 ns e 90 ns de §1.1 e a coluna em ns da tabela de K
  rotulados como alvo derivado da contagem de ciclos, não resultado de síntese. Nenhum ID novo.
- 15/out/2026 (07:20): no tutorial, "Example 16.1, printed in §16.3" virou links — `<a href="#ex-ulf">Example 16.1</a>, shown
  in <a href="#s16-3">section 16.3</a>` —, e a h3 de 16.3 ganhou `id="s16-3"` nas duas edições (o mesmo id que o sumário
  lateral daria). Extração de figuras refeita (`tutorial/figures/`, 43 SVGs inalterados, PASS).
- 15/out/2026 (08:40): remissões viraram links nas duas edições de LF, por `lf-docs/linkrefs.py` (texto idêntico fora das
  âncoras, conferido): §N e §N.M, "Section/section N.M", "Lesson(s) N", "Figure(s) N" e, só no tutorial, "Example N.M".
  Tutorial: 231 seções, 49 lições, 50 figuras, 30 exemplos; referência: 239 seções, 43 figuras (as 6 menções a
  "Example 16.1" da referência ficam em texto: o exemplo só existe no tutorial). Ids novos: toda h3 tem `id="sN-M"`
  (o mesmo que o sumário lateral daria) e toda `<figure>` tem o id estável de `tutorial/figures/manifest.json`. Fora do
  alcance: texto dentro de links, SVG, scripts, código, títulos (h1–h4), rótulos `span.msg` e o rótulo "Figure N —" das
  legendas. Extração de figuras refeita: 43 SVGs inalterados, PASS.
- 15/out/2026 (tarde): **Figura 1 nova** nas duas edições, em §1.1 (`fig-actor-model-message`: o modelo de atores mensagem a
  mensagem, duas execuções, caixa de correio, x termina em 4 ou em 3; 4 passos e uma animação contínua que roda com Play,
  em sincronia com os passos). As antigas 1–43 viraram 2–44; legendas, links e o número escrito dentro do SVG da antiga 38
  (hoje 39) renumerados pelos ids estáveis. No tutorial, FE1 após a figura. Extração refeita (44 SVGs, PASS).
- 16/out/2026: **sem "§" nas remissões** das duas edições de LF (`lf-docs/sectwords.py`). Subseção: "section N.M"; seção de
  topo: "Lesson N" no tutorial e "section N" na referência; maiúscula no início de frase; listas homogêneas viram "sections
  4.10 and 4.12" com cada número como link; o intervalo "4.1–4.13" virou dois links. Texto de código e de SVG recebeu as
  mesmas palavras, sem link. Ficam os rótulos "§ 01 / …" das aberturas da referência. Tutorial: 214 remissões avulsas, 7
  listas, 6 em código, 8 em SVG; referência: 221, 7, 6 e 8. Palavras pelo delta (+222 no tutorial, +229 na referência);
  links +1 em cada edição (o intervalo). `fig-tag-nothing-send-under-both` passou a ter um SVG por edição; o rótulo de
  `fig-ways-establish-tag-safe` foi encurtado para caber ("centralized: reactor-c, HW-RTI (section 9.9)"). O CI recusa
  "§" seguido de número nos HTMLs de LF.
- 16/out/2026 (manhã): **tabelas numeradas.** Toda `<table>` dos cinco HTMLs do projeto tem `id="tab-…"` estável e legenda
  `<caption><b>Table N &mdash; título.</b></caption>`, numerada na ordem do documento (`lf-docs/tablecaps.py`): 12 em cada
  edição de LF (mesma ordem e números nas duas), 32 na especificação, 3 na nota micro-LF, 6 no guia. Remissões vagas
  ("the table", "the fourth row of the table") viraram "Table N" com link: LF, 8 no tutorial e 6 na referência (Tabelas 3,
  7, 11 e 12; a dica do quiz C16.6 também); especificação, 4 (Tabelas 6, 16 e 28). Texto de SVG: "the table of section
  4.13" virou "Table 3 of section 4.13". "grow the table" (fila em trânsito em hardware) virou "grow the in-transit table".
  Palavras pelo delta: +114 tutorial, +110 referência, +227 especificação; links +8, +6, +4. Nota micro-LF: ids de marcador
  duplicados (`mk-n` em 5 SVGs) renomeados e a referência [6] citada. O CI confere as legendas e recusa "the table".
- 16/out/2026 (08:56): **"Section" com maiúscula** em toda remissão numerada das duas edições de LF ("Section 4.7",
  "Sections 4.10, 4.12"), como "Figure", "Table" e "Lesson": 170 no tutorial, 240 na referência, incluindo código, SVG e
  quiz. Mesmo número de caracteres, palavras e links; só o SHA muda. O CI recusa "section N" minúsculo.
- 16/out/2026 (09:09): **"shows", não "draws".** Figura e diagrama "show" (Figures 28, 32 e 34, 43, a legenda da 28, a da
  29 com "shown as points", e o ícone de diagrama do editor, 4× no tutorial). Onde "draw" era expressão idiomática:
  "make the wider distinction" e "states/stated the boundary" (Lesson/Section 7). Tutorial 13 trocas, referência 9;
  palavras e links iguais. O CI recusa draw/draws/drawn/drew em todos os HTMLs.
