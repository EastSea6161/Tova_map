#pragma once
namespace UIModeChoice
{
    const CString UIXAML_M_BACKIMAGE    
        (
        _T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>") 
        _T("  <Border.Background>") 
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>") 
        _T("      <GradientStop Color='#527BA8' Offset='0'/>") 
        _T("      <GradientStop Color='#aac9de' Offset='0.567'/>") 
        _T("    </LinearGradientBrush>")  
        _T("  </Border.Background>")  
        _T("</Border>") 
        );

    const CString UIXAML_M_DEMAND_FALSE
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Select Demand </TextBlock>")    
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'> Stratum(Tijp)</TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString KR_UIXAML_M_DEMAND_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>목적별 기종점 통행자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_DEMAND_DEFAULT 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")    
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Select Demand </TextBlock>")    
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333'> Stratum(Tijp)</TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_DEMAND_DEFAULT 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>목적별 기종점 통행자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);


    const CString
        UIXAML_M_DEMAND_SELECTED 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Select Demand </TextBlock>")    
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366'> Stratum(Tijp)</TextBlock>") 
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_DEMAND_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")     
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>목적별 기종점 통행자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_MODEL_DEFAULT 
        (
        _T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90905'/></TextBlock>")    
        _T("    <Border Height='1' Background='#ddc09e' />")
        _T("    <Border Height='1' Background='#ffffff' />")
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_MODEL_DEFAULT 
		(
		_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90905'/></TextBlock>")    
// 		_T("    <Border Height='1' Background='#ddc09e' />")
// 		_T("    <Border Height='1' Background='#ffffff' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>Logit 모형</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_MODEL_SELECTED 
        (
        _T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")
        _T("    <Border Height='1' Background='#6d896a' />")
        _T("    <Border Height='1' Background='#a1c19c' />")
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_MODEL_SELECTED 
		(
		_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")
// 		_T("    <Border Height='1' Background='#6d896a' />")
// 		_T("    <Border Height='1' Background='#a1c19c' />")
	    _T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>Logit 모형</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_DECISIONTREE_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Decision Tree </TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_M_DECISIONTREE_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Decision Tree </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_M_DECISIONTREE_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Decision Tree</TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );


    const CString
        UIXAML_M_VARIABLE_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Define Variable </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_VARIABLE_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 수단선택 대안 및 </TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 호용함수 설정 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_VARIABLE_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Define Variable </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_VARIABLE_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'> 수단선택 대안 및 </TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'> 호용함수 설정 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_VARIABLE_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Define Variable </TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_VARIABLE_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")     
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>수단선택 대안 및</TextBlock>")  
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>호용함수 설정</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_ADJUSTMENT_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Adjustment </TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_M_ADJUSTMENT_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Adjustment </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_M_ADJUSTMENT_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Adjustment </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_M_OUTPUT_FALSE
        (
        _T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/>  </TextBlock>")    
        _T("    <Border Height='1' Background='#a0a5a8' />")
        _T("    <Border Height='1' Background='#cdd1d4' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_OUTPUT_FALSE
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/>  </TextBlock>")    
// 		_T("    <Border Height='1' Background='#a0a5a8' />")
// 		_T("    <Border Height='1' Background='#cdd1d4' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>출력 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);


    const CString
        UIXAML_M_OUTPUT_DEFAULT 
        (
        _T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#ddc09e' />")
        _T("    <Border Height='1' Background='#ffffff' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_M_OUTPUT_DEFAULT 
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#ddc09e' />")
// 		_T("    <Border Height='1' Background='#ffffff' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>출력 설정</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_M_OUTPUT_SELECTED 
        (
        _T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#46688D' />")
        _T("    <Border Height='1' Background='#7AA5CF' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Output Design</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );

	const CString
		KR_UIXAML_M_OUTPUT_SELECTED 
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>출력 설정</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_SELECT_VARIABLE_FALSE
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Select Variable </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_SELECT_VARIABLE_DEFAULT
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Select Variable </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_SELECT_VARIABLE_SELECTED 
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Select Variable </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_REGION_FALSE
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Region </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_REGION_DEFAULT
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Region </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_REGION_SELECTED 
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Region </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_RUN_ELASTICITY_FALSE
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Run </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'>Elasticity of Demand</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_RUN_ELASTICITY_DEFAULT
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Run </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'>Elasticity of Demand</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_RUN_ELASTICITY_SELECTED 
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Run </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'>Elasticity of Demand</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_VIEW_LOG_FALSE
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> View Log </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_VIEW_LOG_DEFAULT
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> View Log </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_M_VIEW_LOG_SELECTED 
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> View Log </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

}