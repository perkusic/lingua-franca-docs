# Revisão de registro formal — 29/set/2026

Trocas de vocabulário informal por equivalente formal nos três documentos. Colunas: **R** = `lingua_franca_rti.html`, **T** = `lingua_franca_tutorial.html`, **S** = `HW-RTI_project_specification.html`; o número é quantas vezes a troca foi feita. Trechos em `<code>`, `<pre>` e scripts não foram tocados.

## Primeira rodada ("comfort")

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| Prerequisites: comfort with concurrent programming | Prerequisites: familiarity with concurrent programming | 0 | 1 | 0 |
| the least comfortable decision in the file | the decision in the file whose soundness is least certain | 1 | 1 | 0 |
| This one is uncomfortable and should be stated in the report | This point weakens the comparison and must be stated in the report | 0 | 0 | 1 |
| one engineer competent in Chisel and comfortable reading the C runtime | one engineer proficient in Chisel and familiar with the source of the C runtime | 0 | 0 | 1 |

## Terms renamed (keep these names from now on)

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| Cheap condition | Low-cost condition | 1 | 1 | 0 |
| cheap condition | low-cost condition | 3 | 3 | 0 |
| cheap evidence | low-cost evidence | 2 | 2 | 0 |
| cheap path | low-cost path | 0 | 0 | 1 |
| — cheap, and strong, because | — inexpensive and strong, because | 1 | 1 | 0 |
| Not the cheap one | Not the low-cost one | 0 | 1 | 0 |
| Cheap to satisfy on-chip | Inexpensive to satisfy on-chip | 0 | 0 | 1 |
| already insured, more cheaply | already covered, at lower cost | 0 | 0 | 1 |
| TAG(just below | TAG(immediately below | 4 | 4 | 0 |
| just below | immediately below | 6 | 9 | 1 |
| just under | immediately below | 1 | 1 | 0 |
| may run to just short of 30 ms | may run up to the tag immediately below 30 ms | 1 | 1 | 0 |
| when its shortcut fails | when its fast path does not apply | 1 | 1 | 0 |
| the reference's shortcut needs only column e | the reference's fast path needs only column e | 0 | 0 | 1 |
| The shortcut returns | The fast path returns | 0 | 0 | 1 |

## Get / gets / shows up → receive, obtain, appear

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| ordinary software mostly gets to ignore | ordinary software can largely ignore | 1 | 1 | 0 |
| logical time never gets ahead of physical time | logical time never advances ahead of physical time | 1 | 1 | 0 |
| what start tag a latecomer gets | which start tag a late-joining federate receives | 1 | 1 | 0 |
| the one a persistent federate gets if it shows up after | the one a persistent federate receives if it connects after | 1 | 1 | 0 |
| so neither can get one | so neither can receive one | 1 | 1 | 0 |
| a good example of what a reimplementation gets to discard: in hardware both collapse into the same thing, | a clear example of what a reimplementation may discard: in hardware both reduce to the same operation, | 1 | 1 | 0 |
| so a stray client gets a | so that an unexpected client receives a | 1 | 1 | 0 |
| the wait gets a clock deadline | the wait receives a clock deadline | 1 | 1 | 0 |
| Written by hand in C++ and VHDL, this ordering guarantee is the part that is famously hard to get right | Implemented manually in C++ and VHDL, this ordering guarantee is notoriously difficult to establish correctly | 1 | 1 | 0 |
| Most of §4 survives the move, and one part of it gets easier. | Most of §4 carries over unchanged, and one part of it becomes simpler. | 1 | 1 | 0 |
| there is no lock order to get wrong | there is no lock order that can be violated | 1 | 1 | 0 |
| Two things get harder, and they are where the engineering actually lives. | Two aspects become harder, and they are where the engineering effort lies. | 1 | 1 | 0 |
| to get linting | to enable linting | 1 | 1 | 0 |
| If a Windows installation shows up instead | If a Windows installation appears instead | 1 | 1 | 0 |
| A and B get their diagonal entries | A and B obtain their diagonal entries | 1 | 1 | 0 |
| neither can get a TAG for g | neither can receive a TAG for g | 0 | 1 | 0 |
| so A gets no PTAG and waits | so A receives no PTAG and waits | 0 | 1 | 0 |
| When does it get a PTAG, and when does it get nothing? | When does it receive a PTAG, and when does it receive nothing? | 0 | 1 | 0 |
| C gets full TAGs | C receives full TAGs | 0 | 1 | 0 |
| S gets a DNET | S receives a DNET | 0 | 1 | 0 |
| it gets full TAGs | it receives full TAGs | 0 | 1 | 0 |
| Under what condition does it get a full TAG | Under what condition does it receive a full TAG | 0 | 1 | 0 |
| Nothing in how far the iteration gets. | Nothing in how far the iteration progresses. | 0 | 1 | 0 |
| both would show up as DT-1 mismatches | both would appear as DT-1 mismatches | 0 | 0 | 1 |

## Nobody / thing(s) / honest / story → precise nouns and adjectives

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| so nobody is already late on arrival | so that no federate is already late when it arrives | 1 | 1 | 0 |
| “The earliest thing on my queue is tag g.” | “The earliest event on my queue is at tag g.” | 1 | 1 | 0 |
| “the earliest thing on my queue is tag g.” | “the earliest event on my queue is at tag g.” | 1 | 1 | 0 |
| “nobody downstream cares before tag X, stop sending me NETs below it.” | “no node downstream depends on anything before tag X; stop sending me NETs below it.” | 1 | 1 | 0 |
| “Nobody downstream of you cares about anything before tag X, | “No node downstream of you depends on anything before tag X, | 0 | 0 | 1 |
| Two things in that listing are load-bearing. | Two elements of that listing are essential. | 1 | 1 | 0 |
| NETs that nobody needs | NETs that no node needs | 1 | 1 | 0 |
| suppress NETs nobody needs | suppress NETs that no node needs | 0 | 0 | 1 |
| That product is the honest quantification | That product is the precise quantification | 1 | 1 | 0 |
| because it is the sort of thing that looks like duplication and is not | because it appears to be duplication and is not | 1 | 1 | 0 |
| but it is the kind of thing worth knowing before trusting a hang to be your own bug. | but it should be known before a hang is attributed to a defect in one’s own program. | 1 | 1 | 0 |
| it is the honest answer to the question | it is the direct answer to the question | 1 | 1 | 0 |
| Three things to read off it. | Three observations follow from it. | 1 | 1 | 0 |
| Two things Lingua Franca adds: | Lingua Franca adds two elements: | 1 | 1 | 0 |
| an honest measurement | a faithful measurement | 1 | 1 | 0 |
| is used for three things only | supports only three operations | 1 | 1 | 0 |
| because each is built to show one thing | because each is designed to demonstrate one behaviour | 1 | 1 | 0 |
| Three things to find in the code: | Three elements to locate in the code: | 0 | 1 | 0 |
| Honest scope limit. | Scope limit. | 0 | 0 | 1 |
| certification story | certification process | 0 | 0 | 1 |
| the reference does several things Phase 1 deliberately does not | the reference performs several functions that Phase 1 deliberately omits | 0 | 0 | 1 |
| the row worth being honest about | the row that deserves explicit acknowledgement | 0 | 0 | 1 |
| which is real but not the whole story: | which is real but not the complete explanation: | 0 | 0 | 1 |
| it is the first thing anyone blames | it is usually the first cause suspected | 0 | 0 | 1 |
| depend on things the requesting federate | depend on factors the requesting federate | 0 | 0 | 1 |
| would answer a question nobody asked | would answer a question that was not posed | 0 | 0 | 1 |
| So the honest framing of the result | So the accurate framing of the result | 0 | 0 | 1 |
| Enclaves would buy nothing during the phase | Enclaves would offer no benefit during the phase | 0 | 0 | 1 |
| Everything the programmer writes sits inside a generated wrapper | Everything the programmer writes is enclosed in a generated wrapper | 1 | 1 | 0 |
| Pulling on it turns up three more things. | Pursuing it reveals three further issues. | 1 | 1 | 0 |

## Buy / bite / survive / evaporate / knob / spine and other metaphors

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| Only an entry that survives both tests | Only an entry that passes both tests | 1 | 1 | 0 |
| the connection survives, the message does not | the connection is kept and the message is discarded | 1 | 1 | 0 |
| single-inheritance-by-layout trick | single-inheritance-by-layout technique | 1 | 1 | 0 |
| superdense microsteps do not survive passage through real time | superdense microsteps are not preserved across an interval of real time | 1 | 1 | 0 |
| microsteps do not survive passage through real time | microsteps are not preserved across an interval of real time | 0 | 1 | 0 |
| where it is designed to bite | where it is designed to take effect | 1 | 1 | 0 |
| is a wire that is always driven” buys: | is a wire that is always driven” provides: | 1 | 1 | 0 |
| buys availability with a safe-to-advance offset | obtains availability through a safe-to-advance offset | 1 | 1 | 0 |
| Because the whole decentralized argument leans on E | Because the entire decentralized argument depends on E | 1 | 1 | 0 |
| jitter survives the attenuation | jitter remains after the attenuation | 1 | 1 | 0 |
| the cleanest statement of what §10 buys: | the cleanest statement of what §10 provides: | 1 | 1 | 0 |
| the toolchain regenerates all glue | the toolchain regenerates all interface logic | 1 | 1 | 0 |
| bidirectional stalling buys global timestamp order | bidirectional stalling secures global timestamp order | 1 | 1 | 0 |
| What evaporates is the transport | What disappears is the transport | 1 | 1 | 0 |
| The end of a run is another matter, and it survives: | The end of a run is another matter, and it remains: | 1 | 1 | 0 |
| does evaporate | does disappear | 1 | 1 | 0 |
| becomes the knob between latency and area | becomes the parameter that trades latency against area | 1 | 1 | 0 |
| the spine of the rest | the foundation of the rest | 0 | 1 | 0 |
| Lessons 3 and 4 are the spine. | Lessons 3 and 4 are the foundation. | 0 | 1 | 0 |
| and what does it buy? | and what does it gain? | 0 | 1 | 0 |
| an odd sample can miss its deadline | an occasional sample can miss its deadline | 0 | 1 | 0 |
| it is bought by declining several optimizations | it is obtained by forgoing several optimizations | 0 | 0 | 1 |
| The strategy leans on it heavily | The strategy relies on it heavily | 0 | 0 | 1 |
| The risk it buys out is | The risk it would eliminate is | 0 | 0 | 1 |
| R-6 does not bite until M-5 | R-6 does not take effect until M-5 | 0 | 0 | 1 |
| This is the part of the RTI that does not survive a naive translation to hardware. | This is the part of the RTI that a naive translation to hardware does not preserve. | 1 | 1 | 0 |
| turn the knob toward zero and you favor responsiveness while accepting more violation-handler invocations; turn it up and you approach centralized-mode consistency at the price of lag. | reducing the offsets toward zero favors responsiveness at the cost of more violation-handler invocations, and increasing them approaches centralized-mode consistency at the cost of lag. | 1 | 1 | 0 |
| That is the thread worth pulling on. | That is the line of inquiry worth pursuing. | 1 | 1 | 0 |
| Figure 15 — what DNET buys. | Figure 15 — what DNET provides. | 1 | 1 | 0 |

## Sits / lives (location metaphors)

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| A second accommodation sits on the data plane. | A second accommodation concerns the data plane. | 1 | 1 | 0 |
| The rule a federate lives under: | The rule a federate obeys: | 1 | 1 | 0 |
| all distribution lives in shims | all distribution is confined to shims | 1 | 1 | 0 |
| of §4 sits a question | of §4 lies a question | 1 | 1 | 0 |
| The converters live in reactor-c: | The converters are part of reactor-c: | 1 | 1 | 0 |
| D-2 says where the matrix lives; | D-2 says where the matrix is stored; | 0 | 0 | 1 |
| matrix lives in RAM | matrix is stored in RAM | 0 | 0 | 1 |
| the entries live in distributed RAM | the entries are stored in distributed RAM | 0 | 0 | 1 |
| Upstream PTAG propagation lives in | Upstream PTAG propagation is implemented in | 0 | 0 | 1 |
| because propagation lives in the PTAG emission routine | because propagation is implemented in the PTAG emission routine | 0 | 0 | 1 |
| HW-RTI sits between | HW-RTI is placed between | 0 | 0 | 1 |
| the grant sits in the critical path | the grant lies on the critical path | 0 | 0 | 1 |
| see where the RTI sits. | see where the RTI is placed. | 0 | 1 | 0 |
| a smaller delay sits higher | a smaller delay lies higher | 1 | 1 | 0 |
| Unknown sits below the other two | Unknown lies below the other two | 1 | 1 | 0 |
| every grant sits at the largest tag | every grant is at the largest tag | 1 | 1 | 0 |
| skips any that sit in a zero-delay cycle | skips any that belong to a zero-delay cycle | 1 | 1 | 0 |
| whose every immediate upstream sits in its own cycle | whose every immediate upstream belongs to its own cycle | 1 | 1 | 0 |
| sits beside j's NET | appears beside j's NET | 1 | 1 | 0 |
| bit matrices sit beside it | bit matrices are stored beside it | 0 | 0 | 1 |
| therefore sit on consecutive levels | therefore occupy consecutive levels | 1 | 1 | 0 |
| so reactions 1 and 2 sit on consecutive levels | so reactions 1 and 2 occupy consecutive levels | 0 | 1 | 0 |

## Other colloquial phrasing

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| it is simply redundant and dropped | it is redundant and is dropped | 1 | 1 | 0 |
| the conservative assumption is not just made, it is recorded | the conservative assumption is not only made but also recorded | 1 | 1 | 0 |
| the machinery you have just learned | the machinery described above | 1 | 1 | 0 |
| it is easy to attribute to the wrong walk | it is readily attributed to the wrong walk | 1 | 1 | 0 |
| And also, less obviously, the data plane. | Less visibly, the mutex also covers the data plane. | 1 | 1 | 0 |
| It was built, not just published. | It was built, not only published. | 1 | 1 | 0 |
| By hand, start | To launch them manually, start | 1 | 1 | 0 |
| the decision you just learned | the decision described above | 0 | 1 | 0 |
| You have just seen | You have now seen | 0 | 1 | 0 |
| the NET the federate just sent | the NET the federate most recently sent | 0 | 1 | 0 |
| a number that is simply the answer | a number that is itself the answer | 0 | 0 | 1 |
| the reference does not just substitute | the reference does not merely substitute | 0 | 0 | 1 |
| What each step of the walk does is easy to misread | What each step of the walk does is readily misread | 0 | 0 | 1 |
| not just the coordinator? | not only the coordinator? | 0 | 0 | 1 |
| a hang is easy to misattribute | a hang is readily misattributed | 0 | 0 | 1 |
| rare and huge (Figure 2 label and §11 table) | rare and large | 0 | 0 | 2 |

**Total: 248 substituições** (140 expressões distintas).

## Mantidos de propósito (termos técnicos, não informais)

*glue logic* (§9.3), *bit-banged UART*, *smart-grid protection*, *Magic* (número mágico do registrador ID), *pipeline stall* / *stalls*, *hard real-time*, *lets* (verbo neutro), *simply* na pergunta F7, *has just been sent* (temporal), *the kind of evidence* (especificação).

## Aplicada depois, a pedido

| Antes | Depois | R | T | S |
|---|---|---|---|---|
| Ptides supplied the physics; LF supplied the engineering economics. | Ptides provided the physical-time semantics; LF provided a quantitative treatment of its trade-offs. | 1 | 1 | 0 |

## Segunda rodada: precisão (revisão do Angelo, 29/set/2026)

Critério: a frase nova tem de identificar melhor o mecanismo e manter exatamente o alcance da evidência, sem só parecer mais acadêmica.

| Antes | Depois | Onde |
|---|---|---|
| the decision in the file whose soundness is least certain | a design choice for which the source explicitly documents a potential deadlock hazard | R, T (§4.12) |
| the precise quantification of the "broker becomes a bottleneck" critique | an asymptotic characterization of the coordination cost behind the "broker becomes a bottleneck" critique | R, T (§4.10) |
| this ordering guarantee is notoriously difficult to establish correctly — and the class of bug behind the Ingenuity anomaly | the same guarantee has to be enforced explicitly at every crossing of the software–hardware boundary, and a missing enforcement is the class of bug behind the Ingenuity anomaly | R, T (§9.8) |
| resets the microstep to zero, because superdense microsteps are not preserved across an interval of real time | resets the microstep to zero: the reset is part of the tag addition for a positive logical-time delay, and does not depend on how much physical time elapses | R, T (§4.4); variante na resposta C5.1 |
| The risk it would eliminate is already covered | The risk it would mitigate is already addressed | S |
| Ptides provided the physical-time semantics; LF provided a quantitative treatment of its trade-offs. | Decentralized LF builds on Ptides' physical-time semantics and makes the trade-offs among coordination offsets, consistency, availability and latency explicit [22], [37]. | R, T (§5.2) |
| steps back to the largest tag strictly earlier (time − 1, microstep = UINT_MAX) | … the *predecessor*: (t, m − 1) when m > 0, and (t − 1 ns, UINT_MAX) when m = 0 (`lf_tag_latest_earlier`) | R, T (§4.4) |
| immediately below 17 / 23 / 14 / 10 / 20 / 30 ms (prosa, passos e rótulos das figuras) | (N ms − 1 ns, UINT_MAX), the predecessor of (N ms, 0) | R, T, S |
| DNET: "no node downstream depends on anything before tag X; stop sending me NETs below it" | the federate sends a NET for tag g only if g is strictly greater than X, so NETs at or below X are not sent | R, T (§3.3, glossário), S (§0) |
| can stay silent about every next-event tag below X | sends a NET for tag g only if g > X (`federate.c` adds a condition on the last NET sent); a suppressed NET is sent after all if a later DNET falls below it | R, T (§4.7) |
| NETs that no node needs | every NET at or below its DNET threshold / NETs at or below each node's DNET threshold | R, T (§4.9), S (§7.7) |
| sentinel values are load-bearing | sentinel values carry meaning | R, T (§4.4) |
| cheapest one to verify / making any decision cheaper / the cheaper route / a cheaper or stronger one | simplest one to verify / reducing the cost of any decision / the route … with the smaller delay / a lower-cost or stronger one | R, T |
| pseudocódigo: "Phase 1: cheap test" | "Phase 1: low-cost test" (pseudocódigo do documento, não citação do fonte) | R, T (§4.9) |

Quiz (JSON do tutorial; IDs, alternativas corretas e funcionamento preservados, conferido no navegador): C4.3 "get a TAG" → "receive a TAG"; F4 "a TAG up to just below 17 ms" → "a TAG up to (17 ms − 1 ns, UINT_MAX), the predecessor of (17 ms, 0)"; F6 "cheap condition" → "low-cost condition" e "(just below 17 ms)" → "(it gives the predecessor of (17 ms, 0))"; F9 "gets" → "receives" (3×); F10 rótulo do campo "up to just below (ms)" → "up to the predecessor of (t, 0); t in ms"; FB1 "how far the iteration gets" → "progresses"; dica de C14.1 "a cheaper or stronger one" → "a lower-cost or stronger one".

Regra do DNET conferida em `reactor-c` `ccd0512`, `core/federated/federate.c`: o NET é enviado se nenhum DNET chegou ou se `last_DNET < tag` e `last_DNET >= last_sent_NET`; caso contrário vira `last_skipped_NET`, reenviado se um DNET posterior ficar abaixo dele. O microstep é `uint32_t` e o fonte usa `UINT_MAX` (`lf_tag_latest_earlier`, `core/tag.c`); os documentos seguem a grafia do fonte.

## Terceira rodada: verbos de ação e de fala aplicados a coisas (30/set/2026)

Critério: estrutura, regras, mensagens e teoremas não "dizem", "pedem" nem "formam"; usar o verbo que nomeia a relação (depender, determinar, notificar, requisitar, estabelecer).

| Antes | Depois | Onde |
|---|---|---|
| the lessons form four parts | the lessons are organized in four parts | T (How to use) |
| each part opens by saying which lessons its lessons rely on | each part begins by stating the lessons on which its lessons depend | T |
| Part I is needed by everything | Part I is a prerequisite for all later lessons | T |
| Beyond that the dependencies branch rather than run in a line. | Beyond these, the dependencies between lessons are branched rather than linear. | T |
| the reactor model everything later is built on. Every other lesson relies on this part. | the reactor model on which the later lessons are built. Every other lesson depends on this part. | T (Parte I) |
| After them, 5 and 6 go together (6 supplies … that 5 relies on), and so do 7 and 8; either pair can come first. | After them, Lessons 5 and 6 should be read together (Lesson 6 provides … on which Lesson 5 depends), as should Lessons 7 and 8; either pair may be read first. | T (Parte II) |
| Lesson 9 draws on … on all of Lesson 4; Lesson 10 builds on … | Lesson 9 depends on … on the whole of Lesson 4; Lesson 10 depends on … | T (Parte III) |
| the lab that exercises Lessons 4 and 5 | the lab, which applies Lessons 4 and 5 | T (Parte IV) |
| does not say which / does not say which comes first | does not determine which / … which comes first | R, T (§1.1, Figura 1) |
| FIFO delivery per pair says nothing about | … implies nothing about | R, T (§1.1) |
| it says nothing about how the messages of two senders interleave | it does not determine how … | T (resposta) |
| It says nothing about a message in flight | It carries no information about a message in flight | R, T (§4.11) |
| and the message says so. | and the error message states this explicitly. | R, T (§4.11) |
| the theorem says no refinement … could make | the theorem shows that no refinement … can make | R, T (§4.14) |
| What the theory says it costs. | What the theory states about the cost. | R, T (§4.14) |
| a coordinator has to be able to say "wait" | a coordinator must be able to make a component wait | R, T (§4.14) |
| no portable way to say "do this at exactly t" | no portable way to express "do this at exactly t" | R, T (§1) |
| the one place it must say no where the software allocates and says yes | the one case in which it must refuse where the software allocates and accepts | R, T (§9.9) |
| proves it and says so | proves it and signals it with a grant | R, T (§11) |
| and says how it is justified / It says nothing about optimality | and states how it is justified / It makes no claim about optimality | R, T (§15) |
| Where §4.14 tells where the ideas came from, this appendix shows how they work. | §4.14 traces the origin of the ideas; this appendix shows how they work. | R, T (§15) |
| the argument says nothing about what lies above it | the argument establishes nothing about what lies above it | R, T (Figura 32) |
| A says whether, and on which UDP port | A declares whether, and on which UDP port | R, T (Figura 4) |
| tells the RTI that its next event | informs the RTI that its next event | R, T (Figura 7) |
| so A tells B that the port is absent | so A notifies B that the port is absent | R, T (Figura 8) |
| what it tells A's downstreams / What could it wrongly tell C? | what it reports to A's downstreams / … wrongly report to C? | R, T (§4.11; pergunta só em T) |
| *whom* to tell | *whom* to notify | R, T (§4.9) |
| another asks the RTI for that address | another queries the RTI for that address | R, T (§3.3) |
| asks to advance (11×) / ask for g / asks for tag / aria-label "asks for tag g" | requests an advance / request g / requests tag / "requests tag g" | R, T, S |
| cannot tell them apart (texto e rótulo SVG) | cannot distinguish them | R, T (§7, Figura 18) |
| and then asks whether the implementations have fixed it | and then questions whether … | R, T (§4.12) |
| tells the history / says in advance / D-2 says … the access pattern says … | presents the history / states in advance / D-2 specifies … the access pattern determines … | S |
| has to say which one it means / has nothing new to say | must state which one it refers to / conveys no new information | S |
| KPI-2 asks whether / asks a question that barely applies | KPI-2 tests whether / addresses a question that hardly applies | S |
| supplies / supplied / supply / Supplies / hand-supplied | provides / provided / provide / Provides / manually provided | R, T, S (12×) |

Mantidos: "the comment says" e "the source says" (relato do que um texto afirma), instruções de interface ("restart when asked", "then ask you to mark") e "branch" no sentido técnico.

## Quarta rodada: revisão acadêmica (A01–A14), 30/set/2026

Avaliação ponto a ponto do relatório "Academic style and scientific rigor review". Todos os achados foram aceitos; os técnicos foram conferidos antes no fonte (`reactor-c` `ccd0512`) ou executando o código.

| Achado | Decisão | O que mudou | Onde |
|---|---|---|---|
| A01 prova limitada descrita como geral | aceito | FV-6 "will check … for n ≤ 4 and the depth of §10.3"; afirmação para todos os estados exige argumento indutivo; G-6 cita **FV-1** (não P-1) e o que será relatado; métodos de FV-1/FV-6 dizem o alcance; nota "Scope of the formal results" após a tabela da §10.3 | S §1.3, G-6, §10.3 |
| A02 "latência constante" com medida errada | aceito | alvo é a **latência de avaliação K** sob A-1–A-3; grant em [K, Kmax], resposta com fila; legenda da Figura 3 e KPI-2 alinhadas | S §1.3, Fig. 3, §11.2 |
| A03 dispersão medida ≠ limite de E | aceito | estatísticas caracterizam a variação observada; E exige limite justificado (assimetria, deriva, condições); estimativa rotulada como tal; "E é zero por construção" num chip com um oscilador | R, T §6 |
| A04 `lf_delay_strict` confundido com predecessor | aceito, **conferido no fonte** | operações separadas; `lf_delay_strict` só recua com atraso positivo (sem `after` devolve g; `after 0` devolve (t, m+1)); §4.13 corrigido: **M = (10 ms, 0)**; §4.9 com os três casos; legenda da Figura 9 | R, T §4.4, §4.9, §4.13, Fig. 9 |
| A05 domínio do menor ponto fixo | aceito, **conferido executando** | F(D) = min(E, D ⊗ E) a partir do ⊥; nota de que sem E o ⊥ já é ponto fixo; Figura 29 e passos; `fixpoint.py` e Exemplo 15.1 reescritos (saída idêntica, exceto "least fixed point of F") | R, T §15.1, §15.3, Fig. 29; `examples/fixpoint.py` |
| A06 garantias da introdução | aceito | determinismo com estado inicial e entradas etiquetadas iguais e corpos determinísticos; deadlines "checked, with a violation handler"; "logical time not allowed to advance ahead of physical time" no modo padrão (sem `fast`); objetivo da lição 1 | R, T §1, §2; S §1.3 |
| A07 números de desempenho sem status | aceito | 1–3 µs e 10–50 µs rotulados como estimativa de planejamento, com o que o relatório de medição deve conter; "três ordens de grandeza" virou suposição a verificar; razão recalculada **33–100×** para os mesmos pontos (6 ciclos = 30 ns a 200 MHz); "infinitely more on the variance" removido | S §1.2, §9.1, §9.2, §11.1 |
| A08 consequências fortes demais | aceito | "cycle for cycle" sob traços de entrada e reset idênticos (NFR-07); "bimodal by design" → regimes a medir; "lower the mean" separado (DNET vs. trava de segurança); "unassailable" → "strengthen the baseline comparison" | S §1.3, §11.2, §11.3 |
| A09 atribuições causais e de novidade | aceito | frase do Ingenuity na forma sugerida; "Ptides, productized" removido (texto, resposta C7.3 e rubrica no JSON); "no current implementation" limitado ao RTI examinado; HLA acelerado como trabalho relacionado não levantado | R, T §9.8, §5.2, C7.3; S §1.3, §1.4 |
| A10 algoritmo × processo do RTI | aceito | "It never touches data" → cálculo do grant independe de payload; o processo RTI encaminha payloads (§4.12) | R, T §4 |
| A11 terminologia matemática | aceito | SCC "maximal", laço próprio; componente com ciclo = laço de causalidade; aciclicidade é critério estrutural mais estrito que construtividade (Figura 31, §15.3); indução fixa valores e estado, não só status | R, T §15.2, §15.3, Fig. 31, prova da Fig. 32 |
| A12 rótulos do tutorial | aceito | Exemplo 2.2 → "Figure 2 as a program"; selo do Exemplo 7.1 → "compiler rejection verified with lfc 0.13.0" | T |
| A13 retórica | aceito | "hostile to reasoning" → garantias não fornecidas; citação de Lee **conferida e entre aspas** (EECS-2006-1); "network weather" → variação de atraso de rede; "the bug ships" → interleaving errado pode não ser exercitado; objetivo "Compare the ordering and timing guarantees…"; "converges on truth" → a estimativa converge; "costs nothing", "a number you can spend", "falsification point", "kill criterion" reescritos | R, T §1, §6; S §1.3, §11, §14 |
| A14 política editorial e evidências | aceito | regra da terceira rodada substituída pela regra do revisor no handoff (verbo que identifica a relação; sem passada global de sinônimos; shall / presente / is intended to); ref. [5] com permalinks do commit completo; referência da especificação aponta o commit examinado | handoff; R, T ref. [5]; S referências |

Não revertido: as trocas mecânicas das rodadas anteriores continuam, porque nenhuma alterou o sentido; a regra nova vale daqui em diante.
