# Handoff — editing HW-RTI_project_specification.html

Arquivo na pasta conectada: `Lingua Franca\tutoriais\hw-rti\HW-RTI_project_specification.html`
(~256 KB desde 15/out/2026, revisão 0.2; números exatos em `lf-docs/INVENTARIO.md`). A cópia na raiz de `tutoriais\` é antiga — não use.

---

## Contexto mínimo

O arquivo é uma especificação de projeto de engenharia: **HW-RTI**, uma
implementação sintetizável (Chisel/FPGA) da lógica de decisão do RTI do Lingua
Franca — o coordenador que decide quando cada federado pode avançar sua tag
lógica. Escopo da Fase 1: federação on-chip, topologia estática, **alvo n = 8**
(elaboração até n = 32), coordenação centralizada, sem TCP. **Plataforma-alvo desde 8/out/2026: ZCU106**
(XCZU7EV -2, ≥ 250 MHz); o Zynq-7020 fica como referência de área. Decisões de implementação e plano por
etapas: `program/implementation_guide.html`.

Documentos irmãos em `tutoriais\tutorial\` (ver `HANDOFF_lingua_franca_docs.md`).
A §9.9 deles é a ponte explícita para esta especificação: lista o que da álgebra
de concessão sobrevive on-chip, o que evapora e o que fica mais difícil. Vale
reler antes de mexer em §5 (microarquitetura). A §4.13 é o exemplo trabalhado
(matriz preenchida, PTAG, TAG maximal, DNET) e serve de banco de casos para UT
e DT.

`tutoriais\lf-docs\INVENTARIO.md` é a base de conferência: medido a partir dos
arquivos, não dos handoffs. Se este documento discordar dele, este é que está
defasado.

## Convenções do documento

- **Idioma:** inglês. Não misturar idiomas no arquivo.
- **Estrutura:** §0 terminologia, §1 sumário e racional, §2 objetivos, §3
  requisitos, §4 contexto, §5 microarquitetura, §6 tipos, §7 módulos, §8
  interfaces, §9 orçamentos, §10 verificação, §11 avaliação, §12 riscos, §13
  cronograma, §14 entregáveis. 15 h2, 44 h3, 5 h4.
- **IDs numerados** e contíguos, referenciados de forma cruzada: `FR-01..20`,
  `NFR-01..09`, `G-1..6` / `NG-1..7`, `D-1..11`, `L-1..6`, `A-1..4`,
  `UT-1..9` / `DT-1..6` / `FV-1..7` / `ST-1..4` / `PT-1..3`, `KPI-1..6`,
  `R-1..9`, `M-0..9`, `DL-1..7`, `OQ-1..8`.
  Ao adicionar um requisito, adicione também o teste que o descarrega.
- **Todo acrônimo** usado no corpo precisa estar definido em §0 ou expandido no
  primeiro uso. §0.4 também define os símbolos `P_EIMT`, `A`, `R`, `z` e `Q`.
- **Figuras:** 12 SVG inline, `viewBox="0 0 780 H"` — os documentos de LF usam
  760; são duas convenções, não uma. Legendas numeradas sequencialmente. Entradas
  de 28/set/2026: 1 (a pergunta do RTI), 4 (**K, K_max e I numa linha do tempo**
  com duas mensagens, n = 8, uma árvore, z = 2 → K = 24, K_max = 38, I = 34, mais
  K × n com uma árvore e uma por nó; depois do parágrafo "Three quantities, not
  one" de §1.3), 7 (cascata vs. snapshot, §5.3), 8 (corrida LTC/dados, §7.1), 9
  (FSM do sequenciador, §7.2), 11 (pipeline de um NET, §9.1) e 12 (**harness de
  equivalência expandido**, §10: gerar → preâmbulo → mesma entrada → dois modelos
  → normalizar → comparar → aprovado / divergente / excluído, com formas além da
  cor; substituiu a antiga Figura 11). **Há remissões no corpo:** §11.1 cita
  "Figure 2" duas vezes (anatomia do tempo de resposta) — ao renumerar, varra
  `Figure&nbsp;N` fora das legendas.
- **Figuras interativas.** 4, 7, 9, 11 e 12 têm `<svg class="anim">`. Estáticas
  por padrão; um `<script>` inline antes de `</body>` acrescenta os botões ◀ /
  Play / ▶ / All (a 9 e a 11 só têm Play: ponto e cursor contínuos). Sem script,
  com `prefers-reduced-motion` ou na impressão, a figura aparece inteira. O mesmo
  script e o mesmo CSS estão nos documentos de LF.
- **Tabelas (16/out/2026):** 32, todas com `id="tab-…"` e legenda `<caption><b>Table N &mdash; título.</b></caption>`
  numerada na ordem do documento. O texto cita "Table N" com link (`#tab-…`), nunca "the table" (§1.3 → Table 6, §5.2 →
  Table 16, §11.1 → Table 28). Tabela nova: id e legenda, e renumere as seguintes e os textos dos links. O CI confere
  as legendas e recusa "the/this table"; uma estrutura interna se nomeia ("the state table"). As remissões a seções
  continuam com "§" nesta especificação.
- **Figuras "show", nunca "draw"** (16/out/2026; o CI recusa draw/draws/drawn/drew).
- **Figuras com `id` estável (16/out/2026):** `fig-question-rti-answers` … `fig-equivalence-test`, uma por figura; cite
  "Figure N" com link (`#fig-…`). Os títulos das listas de passos são "The steps of Figure N" com link, nunca "the figure
  above"; o CI recusa "the figure/table above/below", "everything above/below" e "described/listed above".
- **Remissões aos documentos irmãos** aparecem como "§N do companion document".
  A numeração de §4 deles mudou em setembro/2026 — ao renumerar lá, varra aqui.
- **Estilo de prosa:** direto, sem ênfase decorativa; afirmações quantificadas
  quando possível; limitações declaradas explicitamente em vez de omitidas. Não
  inventar números: onde não houver medição, dizer de onde vem a estimativa.
- **Paleta CSS** já definida no `<style>` — reutilize `--grant`, `--blue`,
  `--prov`, `--viol`, `--chip`; não introduza cores novas. Uma decisão resolvida
  dentro de uma tabela entra como `<div class="note">` na célula (ver OQ-1).
- **Nomenclatura, em duas camadas.** Na *wire* do LF a mensagem de conclusão de
  tag é `LATEST_TAG_CONFIRMED` (LTC) — é assim que os documentos irmãos a
  chamam. Nos *registradores* desta especificação os campos são `lastTag`,
  `lastPtag`, `completed`. São camadas diferentes e a divergência é deliberada;
  não "corrija" uma para a outra. STA é o offset por federado, STAA o de porta e
  o *violation handler*.
- **Fonte da referência:** `lf-lang/reactor-c`, lido por último no commit
  `ccd0512` (26/set/2026) para §5.3, §7.1, §7.4–§7.7. O metadado "pinned
  commit" do cabeçalho continua vazio — a fixação é tarefa do M-0.

## Decisões registradas

- **OQ-1 — resolvida em 17/set/2026: não.** A Fase 1 continua mirando federados.
  Enclaves apagariam FR-09, FR-10, FR-11 e FR-12 (no `rti_local.c` o PTAG é um
  log e o DNET é vazio) e deixariam FR-14 inalcançável — o M-4 inteiro, que
  carrega o kill criterion do KPI-1. O R-6 já tem mitigação mais barata
  (configuração C-C) e só morde no M-5, porque o harness de §10.2 roda
  `rti_common.c` sozinho. Enclaves servem como gerador de traces no M-0/M-2, não
  como alvo.

- **OQ-6 — resolvida em 28/set/2026: alvo da Fase 1 em n = 8.** Com uma árvore
  de EIMT (P_EIMT = 1): K = 24 ciclos e ~7,0k LUT (13% do Zynq-7020), então
  G-2 e G-5 valem juntas sem relaxar nenhuma; ambas agora são enunciadas em
  n = 8. O projeto continua parametrizado até n = 32 (NFR-05, UT-7); n = 16 é
  sintetizado e reportado no PT-2 como ponto estendido, onde vale o trade-off
  de §5.2. Em n = 8: I = 20 (com ou sem DNET), 10 M msg/s, L_response ≤ 164
  ciclos ≈ 0,82 µs. Rever quando o M-2 trouxer números de síntese. Linha de
  escopo do cabeçalho, G-2, G-5, §5.2, §9.1, §9.2, §9.3 e OQ-6 atualizados.

## Pontos vivos do desenho

- **Alternativa descentralizada (7/out/2026).** Depois da §5.3 dos documentos de LF (micro-LF / `reactor-uc`,
  sem RTI), a especificação passou a responder à objeção "on-chip a coordenação descentralizada basta?":
  termo "Centralized / decentralized" de §0.2 (no reactor-c o RTI ainda faz startup, relógio e shutdown; no
  micro-LF não há RTI), **NG-4 reescrita** (não há o que acelerar; a exclusão não decide se o modo bastaria),
  **linha nova na tabela de §1.4** (no chip E = 0, L limitável por interconexão previsível — o micro-LF já tem
  canal S4NOC —, D de federado em hardware limitável na síntese, então after + W > E + L + D pode valer por
  construção em C-A e, com núcleos previsíveis, em C-C; o que sobra para o HW-RTI: segurança sem limite de tempo,
  que importa para os federados de CPU de C-B, ciclos de atraso zero por PTAG sem hipótese temporal, falha como
  espera e não como valor atrasado), **OQ-7** (a alternativa bastaria para C-A? responder por escrito antes do
  M-4, não é medição) e **R-9** (premissa de §1.1 contestada; mitigação: OQ-7 e relatório que delimita as classes
  de federação). §14.3 passou de 17 para 19 referências (documentação do micro-LF + `reactor-uc` em `df7cfea`;
  Tanneberger et al., ReCPS 2026). Nenhuma seção renumerada. Não pôr o micro-LF como baseline obrigatório da
  §11: plataforma e transporte diferentes, a comparação deixaria de ser entre os mesmos pontos de medição.
  Ajuste do mesmo dia: a linha de §1.4 ressalva o anúncio de entrada de um federado transiente como única mensagem
  que fixa conhecimento sem dado.
- **ZCU106 e o limite com o reactor-chisel (8/out/2026, revisão 0.2).** A nota do ChatGPT
  `program/lf_hardware_coordination_zcu106_expanded.docx` levou a conferir o reactor-chisel em `d94dd2d`
  (5/jun/2024): `Tag` é `UInt` de 32 bits em ns sem microstep (alcance ≈ 2,1 s; NEVER como sem sinal compara acima
  de FOREVER), `Scheduler.scala` linha 48 afirma `TAG ≤ NET` e `CoordinationIO` não tem PTAG. A G-4 ("drop-in") e a
  frase de §8.1 "no change required" eram falsas nesse commit. Resolução: **FR-20** (adaptador de porta: conversão
  de tag relativa a `START_TAG`, entrega de min(TAG, NET), nenhum PTAG, rejeição de federado em hardware dentro de
  ciclo de atraso zero), **D-11** (adaptador em vez de alterar o reactor-chisel), **UT-9**, **OQ-8** (alargar o
  reactor-chisel ou manter o adaptador), §8.1 com lista numerada das três incompatibilidades e permalinks.
  Orçamentos: G-5 ≤ 8k LUT (≈ 3,5% do XCZU7EV; 15% do Zynq-7020), NFR-03 ≥ 250 MHz no XCZU7EV -2, linha nova em
  §9.3 (3,0% / 4,9% / 8,6%). ST-4, PT-2, M-0 (spike no porte ZCU106 que funciona, registrando commits), M-5 e M-6
  passaram a nomear a ZCU106; R-6 confirmado em parte. §14.3: 20 referências. A integração usa o fork
  `erlingrj/lingua-franca` `f60c83c`, cujo submódulo reactor-chisel está em `6c8aecd`, não `d94dd2d`: conferir no M-0.
  O lado de software do Codesign usa o alvo C++, enquanto a especificação supõe federados de software no runtime C —
  pergunta aberta registrada no guia.

- **Custos que a paralelização não remove (15/out/2026, após avaliação externa).** G-3, L-2, D-3 e a linha
  "Notification fan-out" da tabela de §11.1 passaram a dizer que a avaliação paralela remove a serialização do laço de
  software, mas não o custo de distribuir o estado às unidades, da interconexão e arbitragem, do caminho crítico das
  árvores de redução e da atualização consistente dos registradores; esses custos definem frequência, área e parte de
  K e só a síntese (M-2, PT-2) e o KPI-3 os estabelecem. L-2 não diz mais "it is free". Os 150 ns e 90 ns de §1.1 e a
  coluna em ns da tabela de K de §1.3 ficaram rotulados como alvo derivado da contagem de ciclos, não resultado de
  síntese. Não reintroduzir afirmações de ganho por análise assintótica sem esse rótulo. O contrato de transporte de
  hardware do HW-RTI já está em §7.1 (ready/valid por federado em hardware, árbitro round-robin) e A-1 (grants não
  sofrem backpressure); o contrato de um `NetworkChannel` de micro-LF em hardware é assunto da trilha B do guia, não
  da Fase 1.
- **Revisão de 28/set/2026 (tarde) — seis correções.** (1) A definição de ZDC
  em §0.2 não diz mais que ciclo de atraso zero é erro: só laço de causalidade
  é. (3) Contrato de latência em três grandezas: **K** = 10 + ⌈log₂n⌉ + 2⌈log₂d⌉
  + ⌈n/P_EIMT⌉ − 1 (avaliação, constante, toda mensagem), **K_max** = K + (n−1)·z
  (último grant; = K sem ZDC; o merge emite um único grant por nó), DNET em ~n + 5; mensagens sem grant medem-se
  por K. NFR-01, FV-6, PT-1, G-2, KPI-2, §1.3 e §0.4 reescritos. (4) Vazão:
  **I** = K_max − 4, ou max(K_max − 4, n + 1) com DNET (§7.2 explica a
  sobreposição permitida); 14 ou 17 ciclos a n = 16, P_EIMT = n, z = 0 — os
  "8 ciclos" nunca foram sustentados. Espera ≤ (n−1)·I; L_response ≤ (n−1)·I +
  K_max = 273 ciclos ≈ 1,37 µs nesse ponto. §9.2 refeito (6,9 M msg/s com uma
  árvore). (5) FV-1 agora é por ramo: EIMT (plain para a caminhada), fast path
  (g ≤ M), terminação FOREVER (FR-14) e PTAG. (6) G-1 deixou de ser "bit-exact":
  vale sob o **contrato de equivalência** de §5.3 (comparado / abstraído /
  excluído: startup, empate TAG/PTAG, overflow do conjunto em trânsito); DT-1
  exige que exclusões sejam detectadas e reportadas, nunca contadas como pass.

- **§1.4 — fundamentos teóricos (28/set/2026).** Tabela resultado → o que
  estabelece → consequência para o HW-RTI: CMB, HLA/LBTS, teorema de lookahead
  zero (PTAG é forçado — argumento contra a variante "álgebra limpa" da OQ-2),
  ponto fixo síncrono, causalidade estrita (Cataldo), problema do caminho
  algébrico (valida a closure com composição não comutativa; o risco residual
  de R-4 são as flags de ciclo), Donovan et al. 2024 (enunciado de FV-1 contra
  B_i com o termo em trânsito) e CAL. LBTS entrou na tabela de §0.2. §14.3
  passou de 7 para 17 referências. Remete a "§4.14 do companion document".

- **§5.2 e §5.3 (acrescentadas em 28/set/2026).** §5.2: avaliar todos os nós lê
  a matriz inteira a cada mensagem, então ela é *banked by row* em RAM
  distribuída, não BRAM (nota na célula do D-2, Figura 4, L-4 e §9.3
  atualizados). `P_EIMT` = portas de leitura = árvores de redução. §1.3/§9.1
  supunham P_EIMT = n e §9.3 supunha uma árvore; a tabela de §5.2 põe as duas
  lado a lado e, a n = 16, nenhum valor cumpre G-2 e G-5 juntos — **OQ-6**,
  aberta, a decidir com a síntese do M-2. §9.3 ganhou duas linhas (bancos da
  matriz, conjunto em trânsito) e os totais passaram a 7.0k / 11.3k / 19.9k LUT
  (13% / 21% / 37%). §5.3: a referência é uma cascata sequencial, o hardware um
  snapshot. Três regras: máscara de emissão pela linha e de R (**D-8**),
  write-back do start tag no fim da avaliação, *merge* por nó no dispatcher
  (**D-9**, provado por **FV-7**). O DT-1 agora compara, por mensagem e por nó, o
  maior TAG, o maior PTAG acima dele e o DNET; traces abrem com um NET por nó.
- **Segunda revisão de 28/set/2026.** (a) **Merge = só o grant mais forte**
  pela *ordem de autorização* (TAG(y) < PTAG(g) sse y < g; PTAG(g) ≤ TAG(y) sse
  g ≤ y): a referência sempre emite o mais forte e às vezes também os mais
  fracos (TAG(5)+PTAG(10) em ordem crescente, só PTAG(10) na inversa); os mais
  fracos saem da comparação dos dois lados. D-9, FV-7, §5.3, contrato e DT-1
  alinhados; K_max caiu para K + (n−1)·z. (b) **FV-1** separa PTAG da própria
  decisão (g = nextEvent = EIMT, strict > g) do PTAG da caminhada (g = EIMT
  plain de u, strict > g; nextEvent(u) pode ser maior — A⇄B com NETs 5 e 10).
  (c) **FV-3**: conta a partir da aceitação da mensagem, limite c + K_max,
  condições de elegibilidade (sem ameaça externa em ≤ g, acima das guardas,
  no conjunto de emissão); BMC até K_max + I (n = 4, d = 2: K = 14, K_max ≤ 26).
  (d) **DT-1 com preâmbulo**: um NET por nó, não comparado; ao fim compara-se o
  estado normalizado (divergência conta na exclusão (i)); o corpo é comparado.
  (e) **G-3/KPI-3/§1.1/§11.1**: tempo fixo por configuração ≠ independência de
  n; K cresce com n (log com P_EIMT = n, ~linear com uma árvore). (f)
  **RESIGN** entrou em §6.4, §8.1, §5.3 e FR-14: é como cada federado sai no
  fim da execução; sem ele o FOREVER da FR-14 era inalcançável.
- **Figuras de raciocínio (28/set/2026, noite).** Figura 4 nova (K, K_max, I e
  a espera na fila; K × n mostra que "fixo" é por configuração) e Figura 12 (o
  harness com o contrato de §5.3 desenhado: o que se compara, o que se abstrai,
  as três exclusões). Na Figura 6 (microarquitetura), o rótulo "writeback" saiu
  de cima da caixa do propagador de PTAG.
- **Definição de PTAG em §0 (29/set/2026).** Trocada de "do not commit outputs" para: rodam
  as reações cujas entradas em g são conhecidas, saídas e avisos de ausência saem normalmente,
  as que leem entradas ainda desconhecidas esperam, e g só se completa (LTC) quando todas estão
  resolvidas. Mesma formulação dos documentos de LF.
- **Notas passo a passo (29/set/2026).** As figuras 4, 7, 9, 11 e 12 têm, logo após o
  `</figure>`, uma lista numerada (um item por passo); ao passar passo a passo, o item corrente
  aparece numa caixa sob os controles. A 9 (sequenciador) e a 11 (pipeline) ganharam passos para
  isso (estados e estágios); o ponto e o cursor continuam no Play.
- **Empate TAG/PTAG na mesma tag.** Único caso em que a referência depende da
  ordem: TAG da caminhada (EIMT *plain*) contra PTAG da decisão (EIMT
  *tightened*). Com todos os nós conectados plain = tightened (os termos do
  tightening são caminhos já minimizados pela matriz; associatividade e
  monotonia de FV-2), então na Fase 1 o empate só surge depois de um RESIGN no
  fim da execução. O core fica com o PTAG. Vale levantar com o reactor-c.
- **`minInTransit` (§6.3, §7.1, OQ-4, D-10).** O `MSG_SENT` não pode apenas
  baixar `nextEvent(w)` uma vez: a referência reaplica `min(NET, menor tag em
  trânsito)` a cada NET e só libera no LTC. §7.1 agora especifica o conjunto
  como Q tags por nó em RAM distribuída, com um banco de comparadores
  compartilhado; Q = 1 é o registrador único; overflow é erro (FR-18, UT-8) — o
  *merge* (low, high) é seguro mas pode reduzir o progresso num ZDC a um
  microstep por ida e volta. OQ-4 ficou estreitada ao valor de Q. Um `MSG_SENT`
  com g < nextEvent(w) roda a cascata NET inteira. Se o campo mudar de largura,
  **atualize a linha "State table (FF)" da §9.3**, derivada dele.
- **Propagação de PTAG (§7.6, corrigida em 28/set).** A referência **não** usa
  `visited` nessa caminhada (o array é da notificação a jusante que segue um
  LTC); ela termina pelas guardas de redundância. Cada passo visita **todos** os
  upstreams imediatos conectados, não só arestas de atraso zero: TAG(g) se o
  EIMT plain do upstream > g, PTAG(g) se = g e o strict > g; só quem recebe PTAG
  continua. Fontes com tags distintas → caminhadas em ordem crescente; pior caso
  (n−1)·z, z = nós com `inZdc` (§1.3 e §9.1 atualizados). O ponto fixo limitado
  do hardware é mais forte que o original, não uma tradução.
- **Duas guardas em série (§7.5).** A decisão aceita TAG com EIMT ≥ lastPtag,
  mas a camada `notify_*` descarta o valor emitido (latestEarlier(EIMT)) se não
  for > lastPtag — na igualdade o TAG nunca sai. FR-08 atualizado.
- **DNET (§7.7).** Copiar o atalho da referência (candidato do remetente se
  ≤ lastDnet(u)), o corte `< start_time → NEVER`, e NEVER (não `continue`) para
  membro de ZDC ou upstream de remetente em ZDC. A nota de hazard foi corrigida:
  sem transientes a referência hoje concede FOREVER; o clamp em `maxStopTag` é
  do ramo com transientes.
- **Duas referências em C (§10.2, G-1).** `rti_common.c` decide, mas a camada
  `notify_*` existe em duas versões que divergem: o RTI remoto descarta um TAG
  com tag `<=` `last_provisionally_granted`, o local só com `<`. O harness tem de
  ser shimado contra a variante **remota**.
- **Fronteira de fidelidade (§10.2).** O harness precisa de federações de
  referência **sem transientes**: um transiente ausente muda a disciplina de
  concessão (concessões atrasadas; o ramo de desconexão concede o próprio next
  event em vez de FOREVER), e isso apareceria como divergência DT-1 imputável à
  configuração.
- **Baseline da avaliação (§11.1–11.3, acrescentadas em 17/set).** §11.1 diz o
  que o hardware remove, o que não remove e o que é defeito corrigível em
  software; §11.2 mapeia as fontes de variância, incluindo o acoplamento ao
  plano de dados que nenhum benchmark de mensagem pequena expõe; §11.3 fixa o
  baseline — RTI em C na CPU do mesmo SoC sobre CSR, com a ressalva de que parte
  da cauda sairia com travamento por socket. **PT-3** é o teste desse
  acoplamento. Fixar essas escolhas antes do M-4, não depois do primeiro gráfico
  favorável.

## Como quero as edições

- Edite o arquivo diretamente e devolva o arquivo editado.
- Use substituição de string exata; não reescreva o arquivo inteiro.
- Sem resumo longo do que foi feito — uma ou duas linhas bastam.
- Só rode verificação (overflow de SVG, referências órfãs, balanceamento de
  tags) se eu pedir, ou se a edição mexer em figuras ou numeração.
- Edições em lote rendem muito mais que uma de cada vez: cada rodada custa um
  upload e um download do arquivo inteiro.
- **Antes de editar, confira o tamanho contra o inventário.** A pasta está no
  OneDrive e já reintroduziu uma versão antiga por cima de uma nova uma vez
  (as §11.1–11.3 sumiram na reorganização de 18/set e foram restauradas por
  diff).

## Frases úteis para economizar

- "Edite e não verifique, resposta curta."
- "Me dê só o parágrafo reescrito, não mexa no arquivo."
- "Aplique estas 6 mudanças de uma vez: ..."
- "Em §N, substitua exatamente: '<trecho>' por '<trecho>'."
