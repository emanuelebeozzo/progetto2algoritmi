# Secondo progetto del corso di algoritmi estrutture dati (docente Alberto Montresor)

## Topic: problemi NP completi e problemi di approssimazione

Il problema consisteva nel trovare il percoso che attraversasse più anelli possibili (rispettando le regole di attraversamento) in un tempo massimo fissato. 

La soluzione proposta fa uso di diverse tecniche di programmazione: greedy, scelta casuale, backtrack con pruning. 

Nella repository è possibile trovare: il dataset usato per la correzione e valutazione del progetto, il testo del progetto con tutte le specifiche e una breve spiegazione della soluzione proposta. 
Per compilare ed eseguire il codice consultare il file "prog2.testo.pdf". 

## Test del progetto

Per generare le immagini dei vari output generati dall'algoritmo è possibile usare uno script in *Ruby* presente nella cartella test.

Se si desidera utilizzarla è necessario avere installato [ImageMagick](https://imagemagick.org/script/download.php) e una versione di [Ruby](https://www.ruby-lang.org/en/downloads/), il packet manager [RubyGems](https://rubygems.org/), la gemma [Bundler](https://bundler.io/) e successivamente effettuare i seguenti passi:

Clonare il progetto:

$ git clone https://github.com/emanuelebeozzo/progetto2algoritmi.git

Installare le dipendenze 

$ bundle install

Recarsi nella carella test

$ cd test

Ed eseguire il seguente comando

```ruby
ruby generate_paths.rb ../path_to_input.txt ../path_to_output.txt
```

Dove gli argomenti dello script sono il file di input del problema e il relativo file di output generato

In automatico genererà una cartella *img* con all'interno tutti i percorsi generati.
