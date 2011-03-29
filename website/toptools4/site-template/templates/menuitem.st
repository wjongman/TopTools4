$if(it.selected)$
<tr>
  <td colspan="2"><b class="selhead">$it.caption$</b></td>
</tr>
<tr>
  <td>&nbsp;&nbsp;</td>
  <td><a href="$it.downloadurl$">Download</a></td>
</tr>
$else$
<tr>
  <td colspan="2"><b><a class="head" href="$it.pageurl$">$it.caption$</a></b></td>
</tr>
$endif$

