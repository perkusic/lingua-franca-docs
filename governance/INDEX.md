# LinguaFranca — índice

Duas frentes que compartilham uma base: os **documentos de Lingua Franca** (uma
edição tutorial e uma de consolidação técnica) e a **especificação do HW-RTI**,
uma implementação sintetizável em Chisel/FPGA da lógica de decisão do RTI.

A ponte entre elas é a **§9.9** dos documentos de LF — *What a hardware RTI would
have to implement* —, que lista o que da álgebra de concessão sobrevive on-chip,
o que evapora e o que fica mais difícil. Quem for mexer na microarquitetura da
especificação deveria ler aquela seção antes. Desde 28/set/2026 a §9.9 resume
também as §5.2–5.3 e §7.1 da especificação (cascata vs. snapshot, leitura da
matriz, fila em trânsito).

## Onde estão os arquivos

Pasta local conectada, reorganizada pelo Angelo em 18/set/2026:
`C:\Users\Angel\OneDrive - Virtus CEEI UFCG\Cursos\Lingua Franca\tutoriais`

```
tutoriais/
├── README.md                     organização da pasta (fonte do Angelo)
├── tutorial/
│   ├── lingua_franca_tutorial.html    edição didática
│   ├── lingua_franca_rti.html         edição de consolidação técnica
│   ├── examples/                      fontes .lf dos exemplos (C e Python) + README
│   └── HANDOFF_lingua_franca.md       ponteiro para o doc do projeto
├── hw-rti/
│   ├── HW-RTI_project_specification.html
│   └── HANDOFF.md                     ponteiro para o doc do projeto
├── micro-lf/
│   ├── micro_lf_fpga_design_note.html      nota micro-LF/FPGA (texto aceito)
│   └── micro_lf_fpga_design_note_rev1.docx revisões rastreadas da nota do ChatGPT
├── program/
│   ├── implementation_guide.html           guia único de decisões de implementação (8/out)
│   ├── hardware_coordination_overview.html stub que redireciona para o guia
│   └── lf_hardware_coordination_zcu106_expanded.docx nota ZCU106 do ChatGPT (fonte)
├── lf-docs/
│   └── INVENTARIO.md             inventário estrutural medido dos arquivos
└── governance/                   cópias espelhadas dos três documentos do projeto
    ├── INDEX.md
    ├── HANDOFF_lingua_franca_docs.md
    └── HANDOFF_hw_rti_spec.md
```

Tamanhos acima são aproximados; os exatos, com SHA-256, estão no inventário.
`governance/` espelha os documentos `claude/*.md` do projeto: ao alterar um
deles, regrave a cópia e confira o SHA-256.

| Arquivo | O que é | Handoff |
|---|---|---|
| `tutorial/lingua_franca_rti.html` | Referência (~590 KB): ordenação determinística por tempo lógico, do protocolo ao silício. 16 seções (14ª ferramentas, 15ª apêndice de pontos fixos, 16ª micro-LF sem RTI), 48 h3, 44 figuras, 51 referências. | `claude/HANDOFF_lingua_franca_docs.md` |
| `tutorial/lingua_franca_tutorial.html` | Tutorial (~770 KB): mesmo conteúdo técnico mais objetivos, 11 exemplos de código em C e Python, um em Python puro (§15) e um micro-LF (§16; runtime verificado, `.ulf` não compilado), 16 checkpoints, uma pergunta após cada figura (48 no total, incluindo as de exemplo), autoverificação nas 103 perguntas, 5 partes e 2 lab steps; laboratório com 3 programas. | idem |
| `hw-rti/HW-RTI_project_specification.html` | Especificação do HW-RTI (~255 KB, revisão 0.2): Fase 1 on-chip, topologia estática, alvo n = 8 (até 32), centralizada, sem TCP, plataforma ZCU106. 15 seções, 44 h3, 12 figuras, IDs `FR/NFR/D/OQ/R/...`. | `claude/HANDOFF_hw_rti_spec.md` |

## Limpeza

Em 28/set/2026 foram apagados, a pedido do Angelo: os cinco arquivos residuais
da raiz (cópias antigas dos HTMLs e dos ponteiros de handoff), `chatgpt/`
(conversões .md de 18/set), `Claude outputs/` (zips de 18/set) e `_antigo/`
(uma cópia da especificação anterior ao §11). Antes, conferiu-se que a cópia da
especificação na raiz não tinha nenhuma frase ausente da versão corrente além
das substituídas nas edições de 28/set. A pasta agora contém só `README.md`,
`tutorial/`, `hw-rti/` e `lf-docs/`.

## Decisões registradas (28/set/2026)

- **OQ-6 (especificação): alvo da Fase 1 em n = 8.** Com uma árvore de EIMT,
  K = 24 ciclos e 13% do Zynq-7020 — G-2 e G-5 valem juntas; n = 16 vira ponto
  estendido, reportado mas não exigido. Rever com a síntese do M-2.
- **Fontes externas: manter o Google Fonts.** Os três HTMLs carregam Space
  Grotesk, Source Serif 4 e IBM Plex Mono de `fonts.googleapis.com` e não são
  autocontidos; a dependência é aceita e registrada no inventário (§2).

- **Figuras: estáticas por padrão, passo a passo sob pedido (28/set/2026,
  noite).** Nenhuma figura se anima sozinha; um script inline em cada HTML
  acrescenta controles de passo, reprodução e pausa. Figuras novas de raciocínio
  usam o mesmo exemplo (S–A–B–C) sempre que cabe, passos numerados, uma frase
  "What to conclude" na legenda e rótulos/formas além da cor; no tutorial, cada
  figura é seguida de uma pergunta com resposta recolhível.
- **Autoverificação no tutorial (28/set/2026).** As perguntas (63 na época; 80 em
  29/set/2026: 48 conferidas na hora, 32 por rubrica) escondem a
  resposta até o leitor responder: as objetivas conferidas na hora, as demais por rubrica com
  autoavaliação; tudo offline, respostas só no navegador do leitor. Correção por
  IA fica para uma eventual versão publicada no claude.ai.

- **Exemplos de código verificados (28/set/2026).** O tutorial tem 11 programas em C e
  Python, todos compilados e executados com `lfc` 0.13.0 antes de entrar no texto; as
  fontes estão em `tutorial/examples/`. Compilar revelou quatro erros antigos do texto
  (níveis da então Figura 1, hoje 2, STA padrão, sintaxe `tardy`, mensagem de laço de causalidade),
  corrigidos nas duas edições.
- **Apêndice de pontos fixos revisado (29/set/2026, tarde).** Uma revisão externa apontou
  seis pontos, todos aplicados: `tag_add` do `fixpoint.py` agora segue `lf_tag_add`
  (mantém o microstep do segundo operando); reação disparada × dependências resolvidas;
  monotonicidade distinguida de uma sequência que só cresce; o avanço de tempo do RTI
  descrito como avanço seguro, não como ponto fixo (§15.3 "Two fixed points and a safe
  advance"); caminhos simples e ciclos simples na diagonal; procedência dos dois commits
  na ref. [5]. Considerada base estável para a migração depois dos testes no navegador.

- **micro-LF no tutorial (7/out/2026).** §5.3 compara a coordenação descentralizada do micro-LF
  (`reactor-uc`, sem RTI) com a centralizada que o HW-RTI implementa, a partir do código no commit
  `df7cfea` e de micro-lf.org. Conclusão registrada como interpretação: um RTI realiza uma arquitetura de
  coordenação, não é exigido pela semântica do LF; o HW-RTI explora o ponto centralizado. A especificação
  HW-RTI recebeu, no mesmo dia, a comparação correspondente: NG-4 reescrita, linha nova em §1.4, OQ-7 e R-9
  (detalhes em `claude/HANDOFF_hw_rti_spec.md`).
- **Nota de projeto micro-LF/FPGA (7/out/2026).** Nota do ChatGPT revisada contra o `reactor-uc` em `df7cfea`:
  `micro-lf/micro_lf_fpga_design_note_rev1.docx` (revisões rastreadas) e `micro-lf/micro_lf_fpga_design_note.html`
  (texto aceito, figuras ASCII redesenhadas em SVG no estilo dos documentos LF). Projeto separado do HW-RTI; serve
  de evidência para a OQ-7. Não entra no inventário dos três HTMLs.
- **Visão geral do programa (7/out/2026).** `program/hardware_coordination_overview.html`: página de orientação
  (sem requisitos) com as duas trilhas — A, HW-RTI centralizado (especificado); B, motor de entrada micro-LF
  descentralizado (exploratório) —, o que compartilham (primitivas, método de traces diferenciais, vocabulário), onde
  divergem, a decisão que as liga (OQ-7 antes do M-4) e os achados em aberto. Escolhida no lugar de fundir a
  especificação e a nota: são documentos de natureza diferente; fundir só se a OQ-7 concluir que o descentralizado
  basta para C-A. **Substituída em 8/out/2026** pelo guia de implementação (o arquivo antigo virou stub).
- **ZCU106 como alvo e guia único de implementação (8/out/2026).** A nota do ChatGPT
  `lf_hardware_coordination_zcu106_expanded.docx` foi avaliada contra o reactor-chisel em `d94dd2d`. Decisões do
  Angelo: a ZCU106 (XCZU7EV -2) passa a ser o alvo, com o Zynq-7020 como referência de área; a especificação foi
  emendada na hora (revisão 0.2: G-4, FR-20 adaptador de porta, D-11, UT-9, OQ-8, §8.1, §9.3; detalhes em
  `claude/HANDOFF_hw_rti_spec.md`); e um documento único guia as decisões: `program/implementation_guide.html`, no
  estilo da nota micro-LF, com estado dos componentes, registro de decisões DEC-01..14 (decididas, propostas,
  abertas), plano por etapas P0–P6 (trilha A) e B1–B4 (trilha B) com portões, o limite com o reactor-chisel,
  contratos (tag, plano de dados, controle AXI4-Lite, base de tempo), matriz de validação e perguntas abertas.
  O guia **não cria requisitos**: aponta para os IDs da especificação; quando discordar dela, a especificação vale
  e o guia é que está defasado. Em aberto: DEC-05/OQ-8 (alargar o reactor-chisel), DEC-11 (commits casados do
  software de referência), DEC-13 (OQ-7 antes do M-4) e o lado C++ do Codesign.
  **Revisão 2 do guia (8/out/2026, tarde):** §6 nova (espera, tempo e violações na trilha B: conhecimento primeiro;
  limite B_i(g) < T(g) com a regra do reactor-uc em `df7cfea` — um prazo por tag candidata, g + o maior `max_wait` das
  entradas não resolvidas —, letras da §5 do tutorial (E erro de relógio, L latência, D atraso do remetente); exemplo
  trabalhado com a Figura 2) e §9 nova (destino de cada uma das 15 seções da nota ZCU106: absorvida, corrigida ou
  mantida só na nota). DEC-15 (violação visível; sem handler, o participante para — o reactor-uc roda o corpo com o
  valor atrasado) e DEC-16 (regra de espera do reactor-uc primeiro; peer perdido é falha, não ausência), ambas propostas.
- **Lição 16 / §16 — micro-LF sem RTI (15/out/2026, pedido do Angelo).** Seção nova no fim das duas edições de LF
  (Parte V no tutorial; nada renumerado, a especificação não precisa de varredura): um programa de dois federados
  seguido do compilador ao silício, com dez figuras (34–43: anatomia, MSCs de startup e de sincronização de relógio,
  máquinas de estado do startup, do canal TCP e do escalonador, linha do tempo medida, tag silencioso nas duas
  arquiteturas, datapath por federado, camadas e porte) e a 16.6 de plataformas e requisitos de hardware (tabela das
  nove plataformas em `df7cfea`: fonte de tempo e resolução, sono interrompível, seção crítica, canais federados). O
  `ulfc` não pôde ser construído (Maven Central bloqueado aqui e na VM do computador); o Exemplo 16.1 imprime a saída
  de um equivalente em C escrito à mão contra o runtime e diz isso. Interessa ao HW-RTI: a 16.5 e a Figura 42 leem
  a regra de espera do micro-LF como datapath por federado, insumo para a trilha B / OQ-7.

## Regras que valem para tudo

- Corpo dos documentos em **inglês**; conversa em português.
- Edição por **substituição de string exata**, nunca reescrita integral.
- Respostas curtas: uma ou duas linhas descrevendo o que mudou.
- Verificação (overflow de SVG, referências órfãs, tags desbalanceadas) só sob
  pedido, ou quando a edição mexer em figuras, numeração ou citações.
- Mudança técnica nos documentos LF vale para os **dois** arquivos.
- Ao renumerar seções nos documentos LF, varra a especificação: ela cita
  "§N do companion document".
- **Nos documentos LF, remissão se escreve por extenso, com maiúscula e como link:** "Section N.M" ("Sections 4.10
  and 4.12"); seção de topo "Lesson N" no tutorial e "Section N" na referência; nunca "§N" nem "section N" minúsculo
  (inclusive em legendas, quiz, código e SVG). O CI recusa os dois nos HTMLs de LF. A especificação HW-RTI e as notas mantêm a convenção "§".
- **Em todos os HTMLs do projeto, tabela se cita por número e com link:** toda `<table>` tem `id="tab-…"` e legenda
  "Table N — título." numerada na ordem do documento; o texto escreve "Table N" (link para `#tab-…`), nunca "the
  table" nem "the fourth row of the table". O CI confere as legendas e recusa as formas vagas.
- **Vocabulário de figuras:** uma figura ou tabela "shows"; nunca "draws", "drawn" ou "draw the line/distinction"
  em nenhum HTML do projeto (o CI recusa).
- **Sem posição no lugar do nome:** nunca "the figure above", "the table below", "everything above/below",
  "described/listed above" — cite "Figure N", "Table N" ou "Section N.M" com link (o CI recusa). Frases literais em vez
  de metáforas ("rides along", "collapses into", "falls out of", "narrow"); toda figura tem `id`, inclusive na
  especificação, na nota micro-LF e no guia.
- **Afirmações sobre sintaxe ou comportamento do LF se conferem compilando** com o
  `lfc` do tarball de release (baixa do GitHub neste ambiente), não de memória nem da
  documentação antiga.
- Afirmações sobre a implementação do RTI vêm do fonte atual de
  `lf-lang/reactor-c`, não da memória. A referência [5] dos documentos LF registra as
  duas leituras: `a852b20` (30/ago/2026) para o corpo do texto e `ccd0512` (26/set/2026)
  para §4.3, a Figura 4 e §9.9; a especificação cita `ccd0512`. Para micro-LF, `lf-lang/reactor-uc`
  (ref. [49], commit `df7cfea`). `git clone --depth 1` funciona neste ambiente.
- **`lf-docs/INVENTARIO.md` é a base de conferência.** Ele é medido a partir dos
  arquivos, não dos handoffs; se um handoff discordar dele, o handoff está
  defasado. Já pegou um erro real: contagem de figuras atrasada em três.
  Regenerado a cada rodada de edição (o último, 8/out/2026, depois da revisão 0.2 da especificação);
  a primeira regeneração integral foi em 28/set/2026 (tarde): todas as contagens de uma só
  passada, com SHA-256; as contagens antigas de itens de lista incluíam
  `<line>` dos SVGs. Ao mudar um HTML, regenere-o inteiro.

## Notas operacionais

- Em 28/set/2026 o shell do computador voltou a montar a pasta conectada
  (`$HOME/mnt/tutoriais`). Ler, buscar e editar lá dispensa o upload/download do
  arquivo inteiro; apagar ainda exige pedir permissão na sessão.
- **A gravação de volta pode reaproveitar um upload antigo com o mesmo nome.**
  Em 28/set/2026 um commit gravou nos dois HTMLs de LF a versão da manhã em vez
  da atual, embora o arquivo enviado estivesse certo. Corrigido regravando a
  partir de um caminho novo. Depois de cada gravação, confira tamanho e SHA-256
  no disco (o shell do computador lê a pasta) contra a cópia enviada.
- Uma cópia trazida para o container fica em cache: ao reusá-la numa sessão
  posterior, re-traga o arquivo antes, ou você edita uma versão antiga sem
  perceber. Aconteceu em 18/set e foi pego pelo tamanho.
- A pasta fica no OneDrive. Uma reorganização feita fora desta sessão pode
  reintroduzir uma versão antiga por cima de uma nova — foi o que aconteceu em
  18/set com o §11 da especificação, detectado por diff e restaurado. Ao voltar
  a uma sessão depois de mexer na pasta, confira tamanhos contra o inventário
  antes de editar.
