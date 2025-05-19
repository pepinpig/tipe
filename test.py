import cv2
img = cv2.imread("points/images/dodecf0.jpg")
cv2.imshow("Test", img)
cv2.waitKey(0)
cv2.destroyAllWindows()

\appxnote{Algorithme type Moravec}
\note{
Voici la formule utilisée pour calculer la variance dans une direction donnée.

On fait une moyenne des intensités au carré, puis on retire le carré de la moyenne.

Plus cette valeur est grande, plus les pixels autour sont variés. C’est ce qu’on cherche à capturer pour détecter les coins.
}
\scriptsize{
\[
\mathrm{Var}_{(dx, dy)}(x, y) = \frac{1}{N} \sum_{i=-w}^{w} I(x + i \cdot dx,\ y + i \cdot dy)^2 - \left( \frac{1}{N} \sum_{i=-w}^{w} I(x + i \cdot dx,\ y + i \cdot dy) \right)^2
\]
}
\normalsize{
où :
\begin{itemize}
  \item $I(x + i \cdot dx,\ y + i \cdot dy)$ : intensité du $i$eme pixel dans la direction $(dx, dy)$,
  \item $N$ : nombre de pixels valides (dans l’image) dans la fenêtre centrée en $(x, y)$,
  \item $w$ : demi-rayon de la fenêtre .
\end{itemize}
}
