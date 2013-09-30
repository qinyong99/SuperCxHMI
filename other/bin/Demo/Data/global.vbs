Sub newtimer1_OnTimeOut()
	If Tags.ThisNode.TagGroup1.Digital = 1 Then
		Tags.ThisNode.TagGroup1.Analog = (Tags.ThisNode.TagGroup1.Analog + 1) mod 100
	End If
	
	If Tags.ThisNode.TagGroup2.Digital = 1 Then
		Tags.ThisNode.TagGroup2.Analog = (Tags.ThisNode.TagGroup2.Analog + 1) mod 100
	End If
End Sub

