# Handoff — editing the Lingua Franca documents

Arquivos na pasta conectada `Lingua Franca\tutoriais\tutorial`. As cópias na raiz
de `tutoriais\` são antigas — não use.

`tutoriais\lf-docs\INVENTARIO.md` é a base de conferência: medido a partir dos
arquivos, não dos handoffs. Se este documento discordar dele, este é que está
defasado — já aconteceu uma vez, com a contagem de figuras atrasada em três.

---

## Os dois arquivos

| Arquivo | O que é |
|---|---|
| `lingua_franca_rti.html` | Versão de **referência** (~465 KB em 7/out/2026; números exatos em `lf-docs/INVENTARIO.md`). Cabeçalhos `§ 01 / motivation`. Para consulta por quem já conhece o assunto. |
| `lingua_franca_tutorial.html` | Versão **tutorial** (~625 KB em 7/out/2026). Mesmo conteúdo técnico, cabeçalhos `Lesson 1 · motivation`, mais o aparato didático e os exemplos de código. |
| `examples/` | As 20 fontes `.lf` dos exemplos do tutorial (C e Python), `fixpoint.py` (Python puro, apêndice §15) e um README. Criado em 28/set/2026. |

**Regra mais importante: o conteúdo técnico é idêntico nos dois.** Similaridade
de texto normalizado: 0,954 pelo método antigo, 0,922 pelo do inventário
regenerado em 28/set (palavras, sem SVG/CSS) — não compare números de métodos
diferentes; o que difere é andaime pedagógico. Toda
correção técnica, nova subseção ou ajuste de figura vai **aos dois arquivos**,
salvo instrução em contrário.

A divergência 4.9/4.10 foi resolvida em 17/set/2026: as duas subseções foram
portadas para o tutorial e os dois arquivos voltaram a ter o §4 completo.

Aparato exclusivo do tutorial: caixa "how to use this tutorial" após o sumário;
12 caixas `class="obj"`; 15 caixas `class="chk"` (Checkpoint; a 15ª é a do apêndice) com respostas em
`<details><summary>Show answer</summary>`; 2 caixas `class="lab"` (lições 4 e 5);
um "Cost model" extra em §4. Desde 28/set/2026: quatro aberturas de parte
(`<div class="part">`: I Concepts 1–2, II The distributed runtime 3–8, III Into
hardware 9–10, IV Synthesis 11–12), cada uma dizendo de quais lições suas lições
dependem; sumário agrupado por parte (`.toc-groups`, `<ol class="toc grp"
start=N>`); checkpoints extras no meio da lição 4 (rótulos "Checkpoint · 4.1–4.3"
e "· 4.4–4.9") e no fim da lição 9 ("· 9.9"). O checkpoint original da lição 9
fica após 9.7. Nada disso cria heading, então h2/h3 continuam idênticos aos da
referência. Se renumerar lições, atualize as faixas nas aberturas de parte e nos
rótulos dos checkpoints. No texto, "this lesson" onde a referência diz "this
section", "Lesson 1" onde ela diz "§1".

**Sumário:** é um `<ol class="toc">`, que já numera sozinho. O texto da âncora
**não leva número** — `<li><a href="#s3">Federations and the RTI</a></li>`, nunca
`3 &middot; Federations...`, que renderizava "3. 3 · Federations".

## Assunto

Como o Lingua Franca garante ordenação determinística por tempo lógico, do
protocolo distribuído até o silício. Tese central, repetida em cinco camadas: *um
componente só pode avançar para uma tag quando é seguro concluir que nenhuma
entrada com tag anterior ainda pode chegar* — o que muda entre as camadas é
apenas **como** essa segurança é estabelecida (comunicação, suposição, resolução
por porta, estrutura, teorema).

## Estrutura (13 seções/lições + §14 de ferramentas + §15 apêndice de pontos fixos + §16 micro-LF)

1. Por que o LF existe · 2. Modelo de reatores (Figura 2: reator, portas e
**níveis**; a Figura 1 é a de atores × reatores de §1.1) · 3. Federações e o RTI (3.1 início/parada, 3.2 federados
transientes, 3.3 vocabulário da wire) · **4. Coordenação centralizada** (ver
abaixo) · 5. Coordenação descentralizada (5.1 Ptides, 5.2 o que o LF herdou, **5.3 micro-LF: coordenação descentralizada
sem RTI**, 7/out/2026) ·
6. Sincronização de relógios · 7. Compilando uma federação · 8. Dentro de um
federado · 9. Codesign HW/SW (9.1–9.8, mais **9.9 — o que um RTI em hardware
teria de implementar**, a ponte para a especificação) · 10. InterPRET
(10.1 FlexPRET, 10.2 S4NOC, 10.3 o rendezvous) · 11. O invariante unificador ·
12. O laboratório · 13. Glossário e referências · **14. Toolchain** (28/set/2026,
nas duas edições: instalar, compilar, rodar federações, rastros, problemas comuns;
h3 14.1–14.5; msg `§ 14 / toolchain` e `Appendix · toolchain`) · **15. Fixed points, dependency
graphs and federated execution** (29/set/2026, nas duas edições; h3 15.1–15.4; msg
`§ 15 / fixed points` e `Appendix · fixed points`) · **16. micro-LF: federated execution without an RTI** (15/out/2026, nas duas
edições, depois do apêndice; no tutorial abre a **Parte V · Beyond the RTI**, sumário `<ol class="toc grp"
start="16">` depois do grupo Reference; h3 16.1–16.6; msg `§ 16 / micro-lf` e `Lesson 16 · micro-LF`).

**§4 em detalhe** (a seção mais densa, e a que alimenta o HW-RTI; ganhou a
**4.14 — Where the algebra comes from** em 28/set/2026):
4.1 estado por federado · 4.2 as duas condições de segurança · 4.3 por que o PTAG
existe · 4.4 álgebra de tags · 4.5 matriz de atrasos mínimos · 4.6 EIMT ·
4.7 DNET · 4.8 enclaves · 4.9 a função de decisão · 4.10 fan-out, travas e custo ·
4.11 mensagens em trânsito · 4.12 threads e a trava única · **4.13 o exemplo
trabalhado de ponta a ponta**.

A numeração de §4 mudou em setembro/2026: as antigas 4.1–4.7 são hoje 4.4–4.10.
Remissão a "§4.7" com sentido de fan-out/travas é referência velha — hoje é
§4.10, ou §4.12 para a trava.

## O exemplo trabalhado de §4.13

Quatro federados: `S` (fonte com timer) → `A` ⇄ `B` (ciclo de atraso zero, sem
`after` nos dois sentidos) → `C` (`after 10 msec`). A matriz é impressa inteira,
com as entradas diagonais de A e B — é o fato de um nó ser upstream de si mesmo
que produz o empate de EIMT e, com ele, o PTAG. Três momentos: o PTAG liberando o
ciclo, o TAG maximal para C (pede 20 ms, recebe até 30 ms − 1 ns) e o DNET
silenciando a fonte. **Se mexer nos números, refaça a aritmética inteira** — os
três momentos compartilham o mesmo estado.

## Mecanismos do RTI já documentados (para não redescobrir)

**Numeração nas entradas datadas.** Cada entrada abaixo usa os números de figura, pergunta
e contagem do dia em que foi escrita. Para chegar à numeração atual: figuras citadas em
entradas de 28/set ou anteriores somam 1 (a Figura 1 de atores entrou em 29/set, manhã);
as Figuras 26–28 da primeira versão do apêndice são hoje 28–30; e, desde 7/out (Figura 18 nova, §5.3),
qualquer figura ≥ 18 citada numa entrada anterior soma mais 1. Os números atuais estão em
**Convenções** e, medidos, no inventário.

- **Mensagens em trânsito (§4.11).** Fila de tags por federado; `min(NET, menor
  tag em trânsito)` aplicado a **cada** NET, podada no LTC. Inserção condicional:
  se o destino já completou a tag, o RTI registra erro em vez de inserir.
- **Concessões atrasadas (§3.2).** Com transiente ausente a montante, TAG e PTAG
  vão para fila de prioridade drenada por thread conforme o tempo físico alcança
  cada tag. No vencimento a entrada é **reexaminada** e pode ser descartada.
- **Fases do RTI (§3.1).** startup → execution → shutdown; quórum só de
  persistentes; persistente atrasado leva `JOINING_TOO_LATE`; `DELAY_START` = 1 s;
  timeout do stop = 30 s em trinta fatias; `PENDING` bloqueia escrita.
- **A trava (§4.12).** Mutex global **recursivo** (`PTHREAD_MUTEX_RECURSIVE`),
  cobrindo decisão, escritas de socket e encaminhamento em blocos de 256 bytes.
  O próprio fonte adverte contra recursivo + condvar.
- **Propagação de PTAG.** Não usa `visited`; termina pelas guardas de
  redundância. O `visited` é da notificação a jusante que segue um LTC — duas
  travessias diferentes para o mesmo conjunto de destino (§4.10). Cada passo
  visita **todos** os upstreams imediatos conectados, não só os de atraso zero;
  só quem recebe PTAG continua (§4.3 corrigido em 28/set/2026).
- **§9.9 (ampliada em 28/set/2026).** Corrigidos: EIMT lê uma **coluna** da
  matriz, não uma linha; o `visited` não é da propagação de PTAG. Acrescentados
  três parágrafos — cascata vs. snapshot, matriz lida inteira a cada mensagem,
  fila em trânsito como tabela pequena — que resumem §5.2–5.3 e §7.1 da
  especificação sem citá-la pelo nome (os documentos de LF não a mencionam).
- **§4.14 — fundações (28/set/2026).** Chandy–Misra–Bryant (null message =
  NET, lookahead = `after`, matriz = lookahead composto); HLA (LBTS = EIMT,
  NER→NET, TAG; o FIXME de NMR no fonte é o nome IEEE 1516 do NER); teorema de
  lookahead zero de Fujimoto (1997) → PTAG é a promessa das requisições
  "Available" (TARA/NMRA) e não pode ser eliminado; tempo superdenso
  (Maler–Manna–Pnueli) e o nome *tag* (Lee–Sangiovanni-Vincentelli); ponto fixo
  SR e construtividade (Edwards–Lee, Berry, Cataldo et al.); fórmula formal do
  EIMT com a cabeça da fila em trânsito (Donovan et al. 2024); matriz como
  fecho em semianel (Rote); Lamport; CAL. Refs 24–40. O §5.1 passou a citar
  [24], [25] e [27]. No tutorial: objetivo novo na lição 4 e duas perguntas a
  mais no checkpoint final da lição 4 (sem caixa nova — continuam 14).
- **Ciclos de atraso zero × laços de causalidade (§4.3, §7; 28/set/2026).**
  Conferido no compilador (`lf-lang/lingua-franca` `5540fed`): o validador
  rejeita **laços de causalidade** no grafo de reações/portas do programa
  inteiro (no lfc 0.13: "Reaction triggers involved in cyclic dependency"); um **ciclo de federados** com atraso
  total zero é legal se o programa for construtivo — o compilador marca os
  federados como membros de ZDC (`FederateInstance.isInZeroDelayCycle`, que
  "não é o mesmo que detecção de laço de causalidade") e gera reações de porta
  ausente; o RTI os libera com PTAG. Único caso em que ZDC é erro: entre
  enclaves (`CEnclaveGenerator`). O checkpoint da lição 7 foi refeito nesses
  termos. Ref [36] agora cita também a versão publicada (ACM TOMACS 2025).
- **§9.9, segunda revisão (28/set/2026).** A frase que dava RESIGN e o ramo
  de desconexão como "sem significado" foi corrigida: com membresia estática
  somem transientes e FAILED, mas RESIGN e o FOREVER de terminação ficam (a
  especificação exige FR-14). O fan-out "para de custar tempo *por mensagem*";
  o tempo fixo ainda cresce com n da configuração. O merge passou a ser "só o
  grant mais forte" (acompanha a especificação). Rótulo da Figura 18 e o
  checkpoint de 9.9 do tutorial ajustados.
- **Laboratório (§12; 28/set/2026).** Três programas, cada um mostrando uma
  coisa: `FeedbackCentralized.lf` (after 200 ms em B→A: só TAG, sem PTAG),
  `FeedbackZeroDelay.lf` (sem after em B→A, construtivo: PTAGs;
  `send_PTAG`/`receive_PTAG`) e `FeedbackDecentralized.lf` (B com timer; STAA
  só dispara se a mensagem chegar depois de B passar da tag — encurtar o atraso
  não basta, a receita atrasa o envio de A). Lab steps 1 e 2 e o "How to use"
  do tutorial atualizados.
- **Figuras de raciocínio (28/set/2026, noite).** Cinco figuras novas (ver
  Convenções), a Figura 22 (cascata vs. snapshot) passou a dizer "fixed per
  build, and it grows with n", e a Figura 23 mostra por que "fixo" não quer dizer
  "independente de n". Os números das figuras novas são os de §4.13 (momento um),
  de §4.11 (tag 4, NET 10; o grant de C cai de 20 para 14) e do laboratório
  (after 100 ms com D 10, L 5, E 1; after 1 ms com o envio atrasado 50 ms).
- **Exemplos de código (28/set/2026, só no tutorial).** 11 blocos `<div class="ex"
  id="ex-…">`: 2.1 Hello, 2.2 Levels (Figura 1), 2.3 Microsteps, 3.1 TwoFeds, 4.1 a
  federação S–A–B–C com rastro (PTAG a A e B em 0/20/40 ms, TAG a C em 29 999 999 com
  microstep −1, DNET a S), 5.1 Decentralized, 7.1 laço de causalidade rejeitado, 8.1
  Deadline, 12.1–12.3 soluções do laboratório atrás de portão. Abas C/Python (escolha
  lembrada em `localStorage`, chave `lf-tutorial-lang`), abas linha de comando/VS Code,
  botão de copiar, saída real. Mais 4 perguntas "Predict before you run" (P1–P4) no JSON
  do quiz. **Todo exemplo foi compilado e executado com `lfc` 0.13.0** — o tarball de
  release baixa do GitHub neste ambiente (`github.com/lf-lang/lingua-franca/releases/
  download/v0.13.0/lf-cli-0.13.0-Linux-x86_64.tar.gz`; `install.lf-lang.org` é bloqueado);
  Java, gcc, CMake e Python já existem. Ao mudar um exemplo, recompile e rode antes de
  colar a saída. O gerador é `examples.py` + `insert_ex.py` + `ex.css` + `ex.js`.
- **Correções descobertas ao compilar (28/set/2026, nas duas edições).** (a) **Figura 1**
  dizia que as reações 1 e 2 ficam no nível 0: reações de um mesmo reator são ordenadas
  pela declaração, então os níveis são consecutivos (conferido no código gerado:
  níveis 1, 2, 3); rótulos, legenda, pergunta F1 e resposta corrigidos. (b) **STA**:
  o texto dizia "default zero"; no reactor-c atual o padrão é *forever*, e o valor se dá
  com `@maxwait(…)` na instanciação (por conexão, `@absent_after(…)`); sem isso uma
  federação descentralizada sem timer trava. (c) O handler de violação se escreve
  `tardy {=…=}`; `STAA(…) {=…=}` ainda compila no 0.13. (d) A mensagem do `lfc` 0.13
  para laço de causalidade é "Reaction triggers involved in cyclic dependency", não
  "creates a cyclic dependency" (texto de §7 e rótulo da Figura 17). (e) Nos rastros os
  eventos se chamam `Sending PTAG` / `Receiving PTAG`, não `send_PTAG`.
- **Revisão de 29/set/2026.** (a) **PTAG não é "não comprometa saídas"**: sob PTAG as
  reações cujas entradas no tag são conhecidas rodam e enviam saídas e avisos de ausência
  (é isso que resolve o ciclo); o que fica retido são as reações que leem entrada ainda
  desconhecida, e o tag só se completa (LTC) quando todas estão resolvidas. Corrigido no
  glossário, §4.3, §4.14, §8, resposta de C4.3/C6.5 e rubrica de C4.3; a mesma definição
  entrou na tabela de §0 da especificação. Não reintroduza "don't commit". (b) §14.1 ganhou
  o roteiro Windows em seis passos (WSL, ferramentas dentro do Ubuntu, lfc no sistema de
  arquivos Linux, VS Code com a extensão WSL, extensão LF instalada no lado WSL, conferir
  `which lfc gcc cmake python3`) e a nota de que o alvo Python compila uma extensão C.
  (c) Porta 15045 ocupada: `lsof -i :15045` + `kill <pid>` do processo certo, nunca
  `pkill RTI`. (d) P4 e a nota do Exemplo 8.1 dizem "esperado, sem atrasos adicionais" e
  pedem que o aluno meça: deadline depende do escalonamento do sistema. (e) "How to use"
  do tutorial não diz mais que os programas do laboratório não são fornecidos.
- **§1.1 — From actors to reactors (29/set/2026, roteiro do Angelo).** Nas duas edições, fim da
  §1: modelo de atores (Hewitt 1973 [41], Agha 1986 [42]); envio × chegada × seleção ×
  processamento; FIFO por par não ordena remetentes diferentes; exemplo x = 1 com "incrementar"
  e "dobrar" (4 ou 3); determinismo em atores por protocolo/restrição/comutatividade, sem
  garantir efeitos intermediários; projeto orientado a atores ≠ uma semântica (Lee et al. 2003
  [43]); reatores = atores + tags (eventos discretos) + ordem explícita (síncronos). Remete ao
  Exemplo 2.2 (tutorial) ou a `Levels.lf` (referência), sem programa novo. Lista do que a garantia
  cobre (mesmas entradas e tags, ações físicas, tags antes de declarações, saída de terminal não
  é evidência, espera local). Bloco "Further viewing" com o vídeo Lang.NEXT 2012 [44], sem
  atribuir afirmações ao vídeo (não foi assistido). **Figura 1 nova** (atores × reatores, 4
  passos); as antigas 1–24 viraram 2–25, e as duas legendas que citavam a federação de exemplo
  agora dizem "Figure 13". A afirmação da Figura 1 (4 com incremento declarado antes; 3 se a
  duplicação vier em tag anterior) foi conferida com `lfc` 0.13.0 num programa de teste, não
  publicado. Refs [1] e [2] ganharam DOI. Tutorial: C1.3–C1.5 no checkpoint da lição 1 e FA1
  após a figura. **Os IDs do quiz não são renumerados:** as perguntas de figura antigas mantêm
  F1–F24 (hoje após as Figuras 2–25), para não embaralhar respostas salvas no navegador.
- **Figura 4 — entrada na federação (29/set/2026).** Refeita como sequência em 9 passos
  animáveis, conferida no reactor-c `ccd0512`: FED_IDS → ACK/REJECT → NEIGHBOR_STRUCTURE →
  UDP_PORT + sincronização inicial (T1/T3, 10 rodadas com `-c init`) → thread por federado →
  B igual → TIMESTAMP(t_A), TIMESTAMP(t_B) (último persistente: fase startup → execution) →
  start_time = max + DELAY_START (1 s) → TIMESTAMP(start_time) por thread, PENDING → GRANTED.
  Logo **depois** do `</figure>` vem `<p class="figsteps-t">` e `<ol class="figsteps">`, uma nota
  por passo (o detalhamento completo). Ao passar passo a passo, o script de controles (o mesmo
  nos três HTMLs) mostra a nota do passo corrente numa caixa `.figlive` logo abaixo dos botões,
  substituindo-a a cada passo, e destaca o item correspondente na lista; em "All", sem script ou
  na impressão, a caixa some e fica só a lista. Com notas, o Play avança a cada 3,5 s. Para dar
  notas a outra figura animada, basta pôr a mesma `ol` logo após o `</figure>`, um `li` por
  nível `--i`.
- **Notas passo a passo em todas as figuras animadas (29/set/2026).** As 10 figuras com passos
  de cada edição (1, 4, 7, 8, 10, 11, 17, 18, 23, 24) têm a lista "The steps of the figure
  above" logo após o `</figure>` (no tutorial, antes da pergunta "Read the figure"), com
  exatamente um `li` por nível `--i`; a legenda diz que o passo corrente aparece sob os
  controles. **Ao mudar os passos de uma figura, ajuste a lista junto** — o script mostra o
  `li` de mesma posição que o nível.
- **Abertura do §4 (29/set/2026).** A frase "The RTI's side is a purely reactive fixed-point
  computation over the federate dependency graph" foi expandida (reativo, grafo de federados,
  e "Two fixed points and a safe advance"), com as exceções (relógio físico, transientes,
  cláusula em trânsito). Depois da revisão da tarde, o *run* é descrito como avanço seguro e
  monótono, não como ponto fixo de uma função (ver a entrada seguinte).
- **§15 — apêndice de pontos fixos (29/set/2026).** 15.1 ordens e pontos fixos (ordem plana do
  status de porta, monotonia, Tarski, iteração de Kleene; esboço de prova em
  `<details class="proof">`, que abre na impressão), 15.2 os dois grafos (reações × federados,
  SCC de Tarjan, problema algébrico de caminhos), 15.3 "Two fixed points and a safe advance"
  (estrutura e valor de um tag são pontos fixos; o tempo é avanço seguro; tabela e exemplo conjunto), 15.4 o mesmo cálculo em
  descentralizado, enclaves, dentro do federado, hardware, e onde os pontos fixos param. Figuras
  26 (quatro ordens em diagramas de Hasse: tags, uma porta, duas portas, atraso por informação; 4
  passos) e 27 (pontos fixos de uma função monótona, iteração de Kleene em teia e um contraexemplo não
  monótono; 5 passos) ilustram a 15.1; 28 (dois grafos, 4 passos), 29 (fecho como aproximações
  sucessivas, 4 passos) e 30 (ponto fixo de um tag, 5 passos) ficam em 15.2–15.3; todas com notas. Refs [45] Tarski 1955, [46] Tarjan 1972, [47]
  Davey–Priestley 2002. No tutorial: FB1–FB5 (perguntas das Figuras 26–30; prefixo novo para não
  colidir com F1–F24/FA1), checkpoint "Checkpoint · appendix" C15.1–C15.4 e o Exemplo 15.1
  (`fixpoint.py`, Python puro, sem abas de linguagem; o cabeçalho diz "plain Python 3, run with
  Python 3.11"). Em `examples.block()`, `plain=True` desliga o realce de LF e `v=` troca o selo.
- **Revisão externa de 29/set/2026 (tarde), aplicada nas duas edições.** (1) `fixpoint.py`:
  `tag_add` segue `lf_tag_add` (`core/tag.c`, igual em `ccd0512` e no runtime do lfc 0.13.0):
  se b acrescenta tempo, descarta-se o microstep de **a** e **mantém-se o de b**; a versão anterior
  zerava o resultado, o que quebrava a associatividade. O programa agora confere casos com
  microstep (inclusive `after 0` depois de atraso positivo), o agrupamento e a associatividade numa
  grade pequena, e declara que é um modelo didático (atrasos finitos, não negativos; sem overflow,
  NEVER ou outros sentinelas). (2) Resolução × disparo: uma reação roda quando está disparada e suas
  dependências estão resolvidas; com todos os gatilhos ausentes ela não é disparada e não roda;
  uma porta está resolvida quando nenhum escritor possível ainda pode mudá-la (rodou ou foi
  comprovadamente não disparado). Texto de 15.3, Figura 30 (fichas "present", legenda e notas),
  resposta e opção da FB1 e o modelo de portas do Python (três estados, três execuções, uma delas
  a da FB1). (3) Monotonicidade ≠ sequência que só cresce: conclusão da Figura 27 (o caso
  f(2) = 4 da FB5 cresce e para acima de 3). (4) Tempo: 15.3 virou "Two fixed points and a safe
  advance"; tabela, parágrafo "Time", "All three together", Figura 26 (legenda e nota 1) e
  abertura do §4 não chamam mais o avanço do RTI de ponto fixo nem afirmam que as concessões ficam
  no máximo após cada mensagem; tags finitas sem máximo, `FOREVER_TAG` como sentinela, e término
  como questão separada. (5) Diagonal: caminhos simples entre federados distintos, ciclos simples
  na diagonal (A→B→A); conclusão da Figura 29 e parágrafo de estrutura. (6) Ref [5] registra os
  dois commits (`a852b20` e, para §4.3, Figura 4 e §9.9, `ccd0512`). IDs do quiz e âncoras
  preservados.
- **Abertura da §15 com rigor (29/set/2026, noite, nas duas edições).** O parágrafo de abertura virou
  três afirmações numeradas, cada uma com hipóteses e o tipo de justificativa: (1) o fecho termina
  em qualquer ordem e dá os menores atrasos (simples; diagonal = ciclo simples), supondo ⊕
  associativa, monótona e x ≤ x ⊕ d; (2) com corpos de reação determinísticos e grafo de reações
  acíclico, o menor ponto fixo de um tag é total e, por isso, o **único**; acíclico é suficiente, não
  necessário; (3) "grants only climb" é propriedade da implementação, não teorema: por federado e por
  conexão, as tags de TAG e PTAG juntas crescem **estritamente** (guardas em `notify_tag_advance_grant`,
  `notify_provisional_tag_advance_grant` e na fila de grants atrasados, `rti_remote.c` em `ccd0512`;
  no RTI local o TAG só exige `>= last_provisionally_granted`), sem afirmar otimalidade. O parágrafo
  corrigiu duas afirmações excessivas (exemplos "todos na federação de exemplo" e "correspondência
  exata"). Dois `<details class="proof">` novos na §15.3: "Why any relaxation order stops…" (boa
  ordem lexicográfica; remoção de ciclo; desigualdades de aresta) e "Why an acyclic reaction graph
  gives exactly one behaviour…" (ordem topológica; total ⇒ maximal na ordem plana).
- **Figuras dos esboços de prova da §15.3 (29/set/2026, noite, nas duas edições).** Nova **Figura 30**
  (logo após a 29): grafo de quatro nós u, x, y, e (u→x 2, x→e 3, x→y 1, y→x 1, y→e 1 ms; menor u→e = 4
  por u→x→y→e), a recursão de §4.5 em duas ordens (x antes: ∞, 5, 4; y antes: ∞, 4 — conferido
  simulando a recursão), descida lexicográfica, remoção do ciclo x y x (6 → 4) e as desigualdades de
  aresta; 5 passos. A antiga Figura 30 virou **31**. Nova **Figura 32** (após a 31): ordem topológica
  a1, b1, a2; a primeira indecisa (b1) é decidível; PP total ⇒ maximal ⇒ único; ?? no laço não é
  maximal; 4 passos. Remissões nos dois blocos "Why" e na abertura. Tutorial: FB6 (y→e = 3 ms:
  termina em 5 por u→x→e; a ordem "y antes" passa por 6) e FB7 (por que x₀ precisa ser total). O
  gerador das duas figuras (`gen.py`) ficou fora dos documentos.
- **Registro formal (29/set/2026, tarde, nos três arquivos).** Vocabulário informal trocado por
  equivalente formal em cerca de 240 pontos, sem mexer em `<code>`, `<pre>` e scripts. Termos que mudaram
  de nome e devem ser mantidos: **"low-cost condition" / "low-cost evidence" / "low-cost path"**
  (antes "cheap …", §4.2, §4.9, Figura 7 e especificação §7.5); a tag imediatamente anterior se
  escreve **explícita**: "(N ms − 1 ns, UINT_MAX), the predecessor of (N ms, 0)" — o *predecessor* está
  definido em §4.4 ((t, m − 1) se m > 0; (t − 1 ns, UINT_MAX) se m = 0); nunca "just below N ms"; "fast path" em vez de "shortcut" (DNET);
  "receives" em vez de "gets" para grants; legenda da Figura 15 "what DNET provides"; rótulo da
  Figura 2 da especificação "rare and large". Mantidos por serem termos técnicos: glue logic,
  bit-banged, smart grid, magic (número), pipeline stall, hard real-time. **Segunda rodada (precisão, pedida pelo
  Angelo):** critério — a frase nova tem de identificar melhor o mecanismo e manter o alcance da
  evidência, não só soar acadêmica. DNET descrito pela regra do código (NET para g só se g > X; igualdade
  suprime; ver §4.7), nunca como "nobody/no node depends". Texto do quiz no JSON acompanha as revisões
  (IDs e `correct` intactos). Registro completo em `lf-docs/REVISAO_REGISTRO.md`. **Regra editorial em vigor (30/set, após a revisão acadêmica):** escolher o verbo que identifica
  com precisão a relação ou a operação; trocar a redação antropomórfica quando ela obscurece o mecanismo, não
  só porque o sujeito é inanimado ("the theorem states", "the nodes form a component", "the client requests"
  são precisos). Nada de passada global de sinônimos. Requisitos normativos com "shall"; definições e
  propriedades estabelecidas no presente; resultados ainda não verificados com "is intended to", "is expected
  to" ou futuro. Separar a propriedade a estabelecer, o método e o resultado obtido; resultado de BMC vale só
  dentro de n e profundidade checados. Números de desempenho rotulados como medição citada, medição do projeto,
  quantidade derivada ou estimativa de planejamento; razões só entre os mesmos pontos de medição. Não reintroduza "comfort",
  "nobody", "honest", "buys", "survives", "sits/lives" (localização), "knob", "spine".
- **Sumário lateral fixo (30/set/2026, nas duas edições de LF).** Um `<script>` inline antes de `</body>`
  monta `nav.sidenav` a partir do sumário do cabeçalho (`header.hero`: `.toc-part` e `ol.toc a`), com as h3
  de cada seção como subitens; dá `id` às h3 que não têm (`sN-k`), tira `<sup>` dos rótulos e marca a seção
  e a subseção correntes (`aria-current`). CSS em `.sidenav`: aparece só com `min-width:1180px`, desloca
  `.wrap` (`margin-left:max(292px,…)`), some na impressão; sem script ou em tela estreita fica o sumário
  do cabeçalho. **Ao acrescentar seção, basta pô-la no sumário do cabeçalho**; subseções entram sozinhas
  se forem `<h3>` entre o `span.msg` da seção e o próximo. Não aplicado à especificação.
- **Revisão acadêmica de 30/set/2026 (A01–A14), aplicada nos três arquivos.** Pontos técnicos que não se
  pode desfazer: (1) `lf_delay_strict(g, d)` só toma o predecessor quando d é positivo; sem `after` (NEVER)
  devolve g, com `after 0` devolve (t, m + 1) — no §4.13, M = (10 ms, 0); o predecessor é
  `lf_tag_latest_earlier` (§4.4, legenda da Figura 9, §4.9). (2) O fecho é o menor ponto fixo de
  F(D) = min(E, D ⊗ E) a partir do ⊥ (matriz toda FOREVER); sem E dentro de F, ⊥ já seria ponto fixo
  (§15.1, §15.3, Figura 29, `fixpoint.py` e o Exemplo 15.1). (3) Aciclicidade é o critério estrutural do
  compilador, suficiente e mais estrito que construtividade (legenda da Figura 31, §15.3). (4) O E de
  after > E + L + D é um limite justificado; a dispersão medida é estimativa, não pior caso (§6). (5) Na
  especificação, FV-1/FV-6 são BMC com n ≤ 4: resultado vale nesses limites; latência constante é a de
  *avaliação* K sob A-1–A-3; números de software são estimativas de planejamento; razão da §11.1 = 33–100×
  para os mesmos pontos de medição; "kill criterion" → "go/no-go criterion". Tutorial: Exemplo 2.2 refere a
  Figura 2 e o selo do Exemplo 7.1 é "compiler rejection verified with lfc 0.13.0" (o gerador `examples.py`,
  fora dos documentos, precisa receber esse selo por exemplo, não um só para todos). Ref. [5] ganhou
  permalinks do commit `ccd05124e4b2086de045a6857702ae5636da30a6`.
- **§5.3 — micro-LF / `reactor-uc` (7/out/2026, pedido do Angelo, nas duas edições).** Subseção nova no fim de
  §5 (sem renumerar seções; antes da ponte de §9.9). Fontes: código de `lf-lang/reactor-uc` no commit `df7cfea`
  (7/set/2026) e a documentação de micro-lf.org consultada em 7/out (refs [48] documentação, [49] código com
  permalinks, [50] Tanneberger, Jellum, Castrillon, Lee, ReCPS 2026 — alocação estática e plataformas, [51]
  Rengarajan, UCB/EECS-2023-185 — histórico: reactor-c descentralizado em placas embarcadas manteve o RTI para
  startup, relógio e shutdown). Fatos do código que o texto afirma e que não se pode desfazer sem reler o fonte:
  **não há RTI** em `reactor-uc`; `FederateMessage` só carrega dados com tag, startup, shutdown e sincronização de
  relógio, entre vizinhos (federados ligados por conexão) — **não existe mensagem de ausência** (nada como
  PORT_ABSENT ou NET); `last_known_tag` é campo de **`FederatedInputConnection`** (não de `InputPort`, como o
  pedido supunha), começa em NEVER e sobe com cada `TaggedMessage` recebida (tag já com o `after`; em conexão
  física, base = tempo físico do receptor); `acquire_tag(g)`: depois de o relógio chegar a t, se alguma entrada de
  canal conectado tem `last_known_tag < g`, espera até t + W, W = maior `max_wait` dessas entradas (prazo absoluto;
  mensagem interrompe e o escalonador refaz a decisão); `@maxwait` vale na conexão ou na instanciação do federado
  de destino (conexão prevalece), default *forever*; o compilador do micro-LF **não** define `@absent_after`;
  mensagem com tag ≤ tag corrente → "Safe-to-process violation", evento reagendado em (tempo corrente, microstep
  + 1) com a tag pretendida original; reação disparada (ou que lê a porta) roda `tardy` se houver, senão o corpo
  no valor atrasado; buffer pré-alocado cheio descarta a mensagem com erro. Startup: handshake com vizinhos e
  `longest_path` rodadas de propostas (máximo); com clock sync, só o grandmaster propõe (relógio + 100 ms por
  rodada), os outros propõem NEVER; o número de rodadas vem de duas buscas em largura (exato em árvore; o texto
  marca como interpretação que não verificamos topologias cíclicas). Clock sync: grandmaster por
  `@clock_sync(grandmaster=true)`, cada federado sincroniza com o vizinho mais próximo do grandmaster, quatro
  timestamps estilo IEEE 1588, servo PI com ganhos padrão do linuxptp. Interpretações marcadas no texto: condição
  suficiente after + W > E + L + D (com W = 0 volta a de §5; **não** é calculada pelo runtime) e a conclusão de
  que um RTI realiza uma arquitetura, não a semântica. **Figura 18 nova** (centralizado × descentralizado, 4
  passos, com notas); as antigas 18–32 viraram 19–33 (remissões no corpo, nas legendas — 27 → 30 e 32; 32 → 19 —
  e nos plurais "Figures N and M" renumeradas por script). Uma frase no fim do primeiro parágrafo de §9.9 remete a
  §5.3. Tutorial: objetivo novo na lição 5, C7.4 (rubrica) e C7.5 no checkpoint da lição 5 (caixa C7) e FC1 após a
  Figura 18. Terminologia: §5.3 usa `@maxwait`/`@absent_after` e marca STA/STAA/STP como históricos; o resto de
  §5 continua com STA/STAA (não mexido nesta passada). Ortografia americana ("neighbor"). **Correção do mesmo dia (tarde):** há uma terceira fonte de ausência — o anúncio de entrada
  (`JoiningTimeAnnouncement`) de um vizinho que entra numa federação em curso fixa `last_known_tag` das entradas vindas
  dele (`startup_coordinator.c` l. 492–509; o laço indexa `inputs[i]` em vez de `inputs[j]`, escreve sem o mutex e pode
  baixar o campo — defeito aparente, descrito no texto como não confirmado upstream); e desserialização falha ou fila de
  eventos cheia descartam o evento mas avançam `last_known_tag` (só o pool de payload cheio o deixa parado). Texto de
  §5.3, resposta da C7.4 e linha de §1.4 da especificação ajustados. A especificação HW-RTI cita esta §5.3 (termo de §0.2 e linha de §1.4): ao
  renumerar §5, varra a especificação.
- **§16 — micro-LF sem RTI (15/out/2026, pedido do Angelo, nas duas edições).** Fonte: `lf-lang/reactor-uc` em
  `df7cfea` (mesma da §5.3) e micro-lf.org (páginas de plataformas consultadas em 15/out). 16.1 do programa aos
  federados (gerador: um projeto C por federado + script de lançamento, sem RTI; canal padrão TCP em 127.0.0.1);
  16.2 startup observado (log INFO e debug; máquinas de estado do `StartupCoordinator` — UNINITIALIZED, CONNECTING,
  HANDSHAKING, NEGOTIATING, RUNNING; juntar-se: pede o start time e entra em max(tempo lógico dos vizinhos) + 50 ms,
  com passo de relógio — e do `TcpIpChannel`; canal desconectado é ignorado por `acquire_tag`; rodada de clock sync
  RequestSync → SyncResponse(T1) → DelayRequest(T3) → DelayResponse(T4), offset = OWD − (T2 − T1), passo acima de
  100 ms, servo PI Kp 0,7 Ki 0,3, período 1 s; prioridade = saltos até o grandmaster); 16.3 um tag passo a passo
  (h4 Step 1–4) com a máquina de estados do escalonador (eventos de sistema **não** passam por `acquire_tag`);
  16.4 custo da ausência e escolha de W; 16.5 datapath por federado (interpretação); 16.6 plataformas e requisitos
  (API `Platform`: `get_physical_time`, `wait_until`, `wait_for`, `wait_until_interruptible`, `notify`, `Mutex`;
  `NetworkChannel`: `open_connection`, `is_connected`, `send_blocking`, callback ou `poll`; tabela das nove plataformas:
  POSIX, Zephyr (`k_uptime_get`, **1 ms**), RIOT, FreeRTOS (tick; 10 ms no template a 100 Hz), Pico, ESP-IDF, Patmos
  (S4NOC polled), FlexPRET (`rdtime64`, `fp_wait_until`, sem canal federado), ADuCM355 (RTC 32 768 Hz); medição
  própria POSIX x86-64 GCC 13.3 -Os: ~67 KB de código, 2,4 KB data, 24,5–24,9 KB bss por federado; ReCPS [50] sem
  números no texto). **Exemplo 16.1 (`examples/micro-lf/`)**: `Tardy.ulf` **não compilado** — o `ulfc` precisa de
  Gradle/Maven Central, bloqueados aqui e na VM; a saída é de `src.c`/`dst.c`, equivalente escrito à mão com as macros
  do gerador, rodado em 15/out. Medições (uma máquina, loopback): W = 50 ms → tag 400 ausente com lag 50 e `tardy` em
  (400 ms, 1); sem handler o corpo roda em (400 ms, 1) com o valor atrasado; W = 150 ms → valor na tag certa, lag
  100; emissor esparso → lag 150 nas tags silenciosas; com W = 500 ms a tag 200 foi resolvida aos 500 ms pela
  mensagem de tag 400 (mensagem posterior resolve antes do prazo). Anomalia registrada: em 13 de 20 execuções a tag
  600 ms rodou 38–43 ms atrasada; o canal TCP não usa `TCP_NODELAY` e 40 ms é o ACK atrasado do Linux — causa
  plausível (Nagle + ACK atrasado), **não testada** (o Angelo interrompeu o teste com `TCP_NODELAY`). Não afirmar
  como causa confirmada. Uma frase no fim de §5.3 aponta para a lição/§16.
- **Glossário STA / STAA (15/out/2026, após avaliação externa, nas duas edições).** O verbete diz que STA/STAA/STP
  são termos do reactor-c (`@maxwait`, `@absent_after`), que o micro-LF tem só `@maxwait` (federado ou conexão), sem
  `@absent_after` e sem mensagem de ausência, e que conhecer o último tag recebido não é saber que nenhuma mensagem
  anterior ainda pode chegar (remete a §5.3 e §16.3).
- **DNET, lado do federado (§4.7).** O federado só manda NET para tag
  **estritamente maior** que o último DNET; o suprimido vira `last_skipped_NET` e
  é enviado se chegar um DNET mais cedo que ele.

## Convenções

- **Idioma:** inglês. Não misturar idiomas no arquivo.
- **Figuras:** 43 SVG inline desde 15/out/2026 (a Figura 1 é a de atores × reatores; a 18 é a de §5.3; 27–33 são as do apêndice §15; 34–43 são as da §16, todas animadas, numeradas pela ordem de aparição), `viewBox="0 0 760 H"` — a
  especificação irmã usa 780; são duas convenções, não uma. Figura 4 é a Figura 1
  da especificação adaptada a 760. Figuras de **raciocínio passo a passo**, todas
  com o exemplo S–A–B–C quando cabe, passos numerados na legenda e uma frase
  *What to conclude*: 10 (grafo → coluna da matriz → EIMT → EIMT estrito, fim de
  §4.6), 11 (fila em trânsito em quatro quadros, tag 4 e NET 10, §4.11), 17
  (janela de ausência, E + L + D e margem, dentro da janela vs. chegada tardia com
  a receita do laboratório, fim de §5), 18 (centralizado × descentralizado no micro-LF, §5.3), 19 (ciclo de
  federados aberto em reações: permitido vs. laço de causalidade, §7), 25 (K, K_max e I numa linha do tempo
  com duas mensagens, mais K × n com uma árvore e uma por nó, §9.9) e as sete do
  apêndice, 27–33. As legendas das Figuras 10 e 11 citam "Figure 13" (a federação
  de exemplo); a da 27 cita 32 e 30; a da 32 cita 19.
- **Figuras interativas.** Vinte e oito figuras de cada edição de LF têm `<svg class="anim">`
  (1, 4, 7, 8, 10, 11, 17, 18, 19, 24, 25, 27–43), todas com a lista de notas, com passos em `<g class="st" style="--i:k">`. Desde 28/set
  (noite) **nada se anima sozinho**: a figura é estática por padrão, e um único
  `<script>` inline antes de `</body>` acrescenta sob cada uma os botões ◀ /
  Play / ▶ / All. Sem script, com `prefers-reduced-motion` ou na impressão, a
  figura aparece inteira. As legendas terminam com "Use the controls below the
  figure…". Ao editar uma figura animada à mão, mantenha a estrutura de passos.
  O gerador (Python) está fora dos documentos.
- **Perguntas após figura (só no tutorial).** Cada figura é seguida
  de `<div class="fq" data-q="Fn">` (hoje 43 figuras; IDs F1–F24, FA1, FB1–FB7, FC1, FD1–FD10 — FD1 é a da Figura 40, FD2–FD10 as das Figuras 34–39 e 41–43; mais 4 "Predict") ("Read the figure") com uma pergunta do tipo
  "se este valor mudar, qual decisão muda?" e resposta em `<details>`. Não contam
  como checkpoint. Ao acrescentar uma figura ao tutorial, acrescente a pergunta.
- **Autoverificação (só no tutorial; 28/set/2026, aprovada pelo Angelo; hoje são 102: 66 exatas — 49 `me`, 8 `field`, 6 `match`, 3 `ms` — e 36 por rubrica; checkpoint da lição 16 = C16.1–C16.7).** As 63
  perguntas originais (39 de checkpoint, `<li data-q="Cb.n">`, e 24 de figura) têm definição
  no JSON `<script type="application/json" id="quiz-data">`: 33 exatas (`me`
  múltipla escolha, `ms` marcar todas, `field` compara a sequência de números
  digitada, `match` listas suspensas) e 30 `rubric` (texto livre, mínimo de 20
  caracteres, depois 2–3 pontos-chave para marcar covered/partly/missed). Objetivas:
  a resposta abre após acerto, após o 2º erro ou por "Show answer" depois de uma
  tentativa; o 1º erro mostra a dica. Respostas no `localStorage`
  (`lf-tutorial-quiz-v1`), "Try again" por pergunta, "Clear my answers" no "How to
  use", placar por checkpoint. Sem script: `<details>` como antes. **Ao mudar,
  acrescentar ou reordenar uma pergunta de checkpoint, atualize o JSON** — os IDs
  são posicionais (caixa, item) e o script ignora IDs sem definição. O gerador é
  `quizdata.py` + `quiz.js` + `quiz.css` (fora dos documentos).
- **Legendas e remissões.** `<b>Figure N — título.</b>`. Remissões no corpo usam
  `Figure&nbsp;N` e são renumeradas por script (`insert2.renumber`), que **não** toca
  legendas: as legendas que citam figuras (10 e 11 → 13; 27 → 32 e 30; 32 → 19) têm de ser
  conferidas à mão, e o plural "Figures&nbsp;N and M" também (o `insert_uc.py` de 7/out já trata os dois casos). O rótulo da Figura 6 (a decisão de concessão) ainda diz "through the
  zero-delay cycle"; é o caso ilustrado, não a regra.
- **Títulos:** h2 por seção, h3 numerada por subseção, **h4** para passos dentro
  de uma subseção (usado em §4.13). A regra CSS de h4 foi copiada da
  especificação irmã.
- **Citações:** 51 referências em §13 (24–40 com a §4.14; 41–44 com §1.1; 45–47 com o apêndice §15; 48–51 com §5.3), âncoras `id="refN"`, chamadas como
  `<sup class="c"><a href="#refN">[n]</a></sup>`. Invariante: toda chamada
  resolve e toda referência é citada ao menos uma vez.
- **Acrônimos:** definidos no glossário de §13 ou expandidos no primeiro uso.
- **Nomenclatura:** STA para o offset por federado, STAA para o de porta e para o
  *violation handler*; STP só onde o texto explica que é o nome antigo. Exceção desde 7/out: §5.3 (micro-LF)
  usa os nomes atuais, `@maxwait`/`@absent_after` e `tardy`, e trata STA/STAA/STP como terminologia histórica. A
  mensagem de conclusão de tag é `LATEST_TAG_CONFIRMED` (LTC) — na especificação
  irmã os registradores se chamam `lastTag`/`lastPtag`, camada diferente, e a
  divergência é deliberada.
- **Fonte primária:** `lf-lang/reactor-c` (`core/federated/RTI/rti_common.c` e
  `.h`, `rti_remote.c`, `rti_local.c`, `core/federated/federate.c`,
  `clock-sync.c` e `.h`, `network/api/net_common.h`,
  `low_level_platform/impl/src/lf_POSIX_threads_support.c`). A referência [5] registra
  as duas leituras: `a852b20` (30/ago/2026) para o corpo do texto e `ccd0512` (26/set/2026)
  para §4.3, a Figura 4 e §9.9; `lf_tag_add` (`core/tag.c`) é igual em `ccd0512` e no runtime
  gerado pelo lfc 0.13.0 dos exemplos. Esse código **muda com frequência**: leia o
  fonte atual em vez de confiar na memória; `git clone --depth 1` funciona neste
  ambiente. Para §5.3: `lf-lang/reactor-uc` no commit `df7cfea` (7/set/2026) — `src/federated.c`,
  `src/environments/federated_environment.c`, `src/schedulers/dynamic/scheduler.c`, `src/startup_coordinator.c`,
  `src/clock_synchronization.c`, `src/shutdown_coordinator.c`, `external/proto/message.proto` e, no compilador,
  `ulf/.../UcConnectionGenerator.kt` e `AttributeSpec.java`; a documentação é micro-lf.org.
- **Paleta CSS** já definida no `<style>`: reutilize `--grant`, `--blue`,
  `--prov`, `--viol`, `--chip`; não introduza cores novas.
- **Estilo de prosa:** direto, sem ênfase decorativa; mecanismos explicados pelo
  que fazem, não só pelo nome; limitações declaradas em vez de omitidas. Não
  inventar números: onde não há medição publicada (o E da sincronização, por
  exemplo), dar ordens de grandeza com procedência e dizer como medir.

- **Extração das figuras (15/out/2026, passo 1 da modularização).** `tutorial/figures/` tem os 43 SVGs (`svg/<id>.svg`,
  ID estável sem número), esqueletos, `manifest.json`, `extract.py`, `assemble.py` e `REPORT.md`. **Os HTMLs continuam
  canônicos.** Depois de qualquer edição num HTML, rode no computador `python3 extract.py ../lingua_franca_tutorial.html
  ../lingua_franca_rti.html` e `python3 assemble.py --check` (deve dar PASS). Nunca transfira `.svg` pelo canal de
  entrega: ele insere metadados C2PA; regenere localmente. Âncoras explícitas: `id="s16-3"` na h3 de 16.3 (o mesmo que o
  sumário lateral geraria); o tutorial liga "Example 16.1" (`#ex-ulf`) e "section 16.3" (`#s16-3`) em 16.1.

- **Remissões como links (15/out/2026, nas duas edições).** "section N.M", "Lesson(s) N", "Figure(s) N" e, no
  tutorial, "Example N.M" são `<a href="#…">`: seções `#sN`, subseções `#sN-M` (toda h3 tem esse id fixo, igual ao que o
  sumário lateral daria), figuras `#<id estável>` (toda `<figure>` tem `id` do `figures/manifest.json`), exemplos `#ex-…`.
  **Texto novo:** escreva a remissão já como link no mesmo formato; `lf-docs/linkrefs.py` faz isso num arquivo sem links,
  mas recusa rodar de novo num arquivo já processado. Ao renumerar figuras, os hrefs não mudam (ids estáveis); só o
  texto do link muda. Figura nova: dê-lhe `id` estável na `<figure>`.

- **Repositório GitHub (15/out/2026).** `perkusic/lingua-franca-docs`, público, branch `main`: cópia byte a byte da pasta
  (sem os `.docx` do ChatGPT nem `Claude outputs/`). Cada push roda `.github/workflows/check.yml`
  (`assemble.py --check` e `.github/scripts/check_html.py`: ids duplicados, links internos sem destino, referências
  sem citação). `extract.py` agora identifica as figuras pelo `id` da `<figure>` (não pela posição) e `assemble.py`
  nomeia placeholders desconhecidos, duplicados, ausentes ou fora de ordem. OneDrive e GitHub são cópias
  independentes: ao editar, atualize as duas e confira os SHA-256.

- **Figura 1 nova e renumeração (15/out/2026, tarde).** Em §1.1, `fig-actor-model-message` (o modelo de atores mensagem a
  mensagem; FE1 no tutorial) é a nova Figura 1; **as antigas 1–43 são hoje 2–44** — em qualquer entrada deste handoff ou do
  inventário datada antes desta, some 1 ao número de figura citado. A renumeração foi feita pelos ids estáveis (legendas,
  textos dos links e o "Figure N" escrito dentro do SVG da sincronização de relógio). **Animação contínua:** a figura usa
  elementos `class="tok"` com `@keyframes` num `<style>` dentro do próprio SVG (prefixo `fa-`); o script de controles só os
  anima com Play, e o CSS da página os congela fora do Play, na impressão e com `prefers-reduced-motion`. O ciclo dura 14 s
  = 4 passos × 3,5 s, para coincidir com o avanço dos passos no Play; o quadro 0% é o estado estático (mensagens na caixa
  de correio, x = 1). Para outra figura animada, siga o mesmo esquema: ciclo = passos × 3,5 s e quadro 0% = figura estática.

- **Remissões sem "§" (16/out/2026, regra permanente).** Nos documentos de LF, toda remissão é escrita por extenso e
  como link: subseção → `<a href="#sN-M">Section&nbsp;N.M</a>`; seção de topo → `<a href="#sN">Lesson&nbsp;N</a>` no
  tutorial e `<a href="#sN">Section&nbsp;N</a>` na referência; **"Section" sempre com maiúscula** (como Figure, Table e
  Lesson; regra de 16/out, 08:56), também no meio da frase e entre parênteses; lista → "Sections <a>4.10</a> and
  <a>4.12</a>" (a palavra fora do link, cada número com o seu); intervalo → "Sections <a>4.1</a>–<a>4.13</a>".
  Vale também para legendas, perguntas do quiz, comentários em blocos de código e texto dentro de SVG (lá, sem link; como os
  SVGs são compartilhados, prefira "Section N.M", que serve às duas edições — "Lesson N" num SVG obriga a guardar uma
  variante por edição, como em `fig-tag-nothing-send-under-both`). Únicas exceções: os rótulos de abertura "§ 01 / …" da
  referência e os documentos com convenção própria (especificação HW-RTI, notas de projeto, guia de implementação), que
  continuam com "§". **O CI recusa** "§" seguido de número nos dois HTMLs de LF (`check_html.py`; a especificação roda com
  `--allow-section-sign`). Conversão de um arquivo antigo: `lf-docs/sectwords.py` (`--lessons` para o tutorial). Ao
  alongar um texto de SVG, confira que ele cabe (houve um rótulo encurtado na Figura 19).

- **Tabelas numeradas e citadas (16/out/2026, regra permanente, todos os HTMLs do projeto).** Toda `<table>` tem
  `id="tab-…"` estável e, como primeiro filho, `<caption><b>Table N &mdash; título.</b></caption>`, numerada 1, 2, … na
  ordem do documento (as duas edições de LF têm as mesmas 12 tabelas, com os mesmos números). **O texto nunca aponta para
  "the table", "this table" ou "the fourth row of the table":** escreve `<a href="#tab-…">Table&nbsp;N</a>` (também em
  legendas, perguntas e dicas do quiz; dentro de SVG, "Table N" sem link). Tabela nova: dê `id` e legenda e renumere as
  seguintes, junto com os textos dos links (os hrefs não mudam). `lf-docs/tablecaps.py` numera um arquivo sem legendas.
  O CI recusa tabela sem `id` ou sem legenda na ordem certa e recusa "the/this/that table" no texto; para uma estrutura
  interna (fila, tabela de estado), escreva o nome dela ("the in-transit table", "the state table").

## Como quero as edições

- Edite os arquivos diretamente e devolva os arquivos editados.
- Substituição de string exata; não reescreva o arquivo inteiro.
- Aplique aos dois arquivos quando a mudança for técnica; diga numa linha se
  aplicou só a um.
- Sem resumo longo — uma ou duas linhas bastam.
- Só verifique (overflow de SVG, referências órfãs, balanceamento de tags) se eu
  pedir, ou se a edição mexer em figuras, numeração ou citações.
- **Legenda curta; explicação no texto (16/out/2026, regra permanente).** A `<figcaption>` tem só o título
  ("Figure N &mdash; título.") e, se preciso, uma ou duas frases de escopo, fonte ou legenda de cores (até ~300
  caracteres). Nunca: passos numerados "(1)…" (vão só na lista "The steps of Figure N", que o Play usa), "What to
  conclude" e instruções de uso ("Use the controls below the figure…", removida de todas as figuras). Depois da lista de
  passos (ou da figura, se não houver lista) vêm, quando necessários, `<p><strong>How to read <a>Figure N</a>.</strong> …</p>`
  (como ler o desenho) e `<p><strong>What <a>Figure N</a> shows.</strong> …</p>` (a conclusão). O CI recusa legenda longa
  ou com "(1)", "What to conclude" ou "Use the controls".
- **Nada de "above/below" para apontar texto, figura ou tabela; nada de metáfora no lugar do fato.** Não escreva "the
  figure above", "the table below", "everything above/below", "described/listed above"; nomeie com link ("The steps of
  Figure&nbsp;N", "Table N", "Sections 4.1–4.14"). Prefira a frase literal à imagem: não "rides along", "collapses into",
  "falls out of", "in miniature", "in one number", "narrow interface", "the arc of everything below" — diga o que
  acontece, quem faz e onde. "That is…" só quando o referente da frase anterior é inequívoco. O CI barra as formas
  posicionais; a clareza do resto é conferida na revisão.
- **Figura e tabela "show", nunca "draw":** "Figure 28 shows both", não "draws"; nem "drawn as", nem a expressão "draw the
  line/distinction" (use "states the boundary", "makes the distinction"). O CI recusa draw/draws/drawn/drew.
- **Tabelas: sempre "Table N" com link, nunca "the table"; toda tabela com `id` e legenda numerada.** O CI barra.
- **Remissões: "Section N.M" (e "Lesson N" no tutorial), sempre com maiúscula e como link; nunca "§N" nem "section N"**
  — inclusive em legendas, quiz, código e SVG. O CI barra o "§" e o "section" minúsculo.
- Ao renumerar seções, **varra também a especificação HW-RTI**: ela cita seções
  destes documentos ("§N do companion document").
- Edições em lote rendem muito mais que uma de cada vez: cada rodada custa um
  upload e um download do arquivo inteiro.
- **Antes de editar, confira o tamanho contra o inventário.** A pasta fica no
  OneDrive e já reintroduziu uma versão antiga por cima de uma nova.

## Frases úteis para economizar

- "Edite e não verifique, resposta curta."
- "Aplique só no tutorial." / "Aplique nos dois."
- "Me dê só o parágrafo reescrito, não mexa nos arquivos."
- "Aplique estas 6 mudanças de uma vez: ..."
- "Em §N, substitua exatamente: '<trecho>' por '<trecho>'."
- "Adicione uma pergunta de checkpoint na lição N sobre X." (só tutorial)
