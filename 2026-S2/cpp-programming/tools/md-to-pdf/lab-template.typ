#let repo-url = "https://github.com/mvodya/vvsu-study/tree/master/2026-S2/cpp-programming"
#let accent = rgb("#1557a6")
#let accent-soft = rgb("#1557a6").transparentize(72%)
#let code-bg = rgb("#111827")
#let sans-font = ("Arial", "Helvetica")
#let mono-font = ("Menlo", "Courier")
#let right-strip-width = 27mm

#let page-background(title, number, source-hash) = context {
  let page-number = counter(page).get().at(0)

  if page-number == 1 {
    place(top + left, rect(width: 100%, height: 31mm, fill: accent))
    place(top + left, dx: 24mm, dy: 9mm)[
      #block(width: 138mm)[
        #text(font: sans-font, fill: white, size: 16pt, weight: "bold")[#title]
      ]
    ]
  } else {
    place(top + right, rect(width: right-strip-width, height: 31mm, fill: accent))
  }

  place(top + right, rect(width: right-strip-width, height: 100%, fill: accent-soft))

  place(top + right, dx: -6mm, dy: 5.4mm)[
    #align(right)[
      #text(font: mono-font, fill: white, size: 6.6pt, weight: "regular", tracking: 0.45pt)[лабораторная\ работа]
      #linebreak()
      #text(font: mono-font, fill: white, size: 22pt, weight: "bold")[#number]
    ]
  ]

  place(bottom + right, dx: -3.8mm, dy: -5mm)[
    #text(font: mono-font, fill: accent, size: 7pt, weight: "medium")[#source-hash]
  ]
}

#let lab(title: "", number: "--", source-hash: "", body) = {
  set document(
    title: title,
    author: ("Mark Vodyanitskiy",),
    keywords: (repo-url,),
  )

  set page(
    paper: "a4",
    margin: (top: 12mm, bottom: 18mm, left: 24mm, right: 43mm),
    background: page-background(title, number, source-hash),
  )

  set text(font: sans-font, size: 11pt, lang: "ru", fill: rgb("#182033"))
  set par(justify: true, leading: 0.58em)
  set list(indent: 1.25em, body-indent: 0.5em)
  show heading.where(level: 1): it => block(above: 1.0em, below: 0.45em)[
    #text(fill: accent, size: 15pt, weight: "bold")[#it.body]
  ]

  show heading.where(level: 2): it => block(above: 0.85em, below: 0.35em)[
    #text(fill: rgb("#23324f"), size: 13pt, weight: "bold")[#it.body]
  ]

  show raw.where(block: true): it => block(
    width: 100%,
    fill: code-bg,
    radius: 5pt,
    inset: 8pt,
    above: 0.55em,
    below: 0.75em,
  )[
    #text(font: mono-font, size: 9.2pt, fill: rgb("#e5edf7"))[#it]
  ]

  v(30mm)
  body
}
